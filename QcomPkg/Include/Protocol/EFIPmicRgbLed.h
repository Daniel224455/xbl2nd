/** 
  @file EFIPmicRgbLed.h
  @brief PMIC RGB LED UEFI Protocol definitions
*/
/*=============================================================================
  Copyright (c) 2012 - 2018 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 08/05/16   va      Adding Charger led protocol 
 11/05/15   al      Syncing to latest
 06/23/14   al      Added GetStatus 
 06/19/14   al      Fixed for red segment
 07/24/25   al      Add RGB voltage source for PMI8962
 03/15/13   al      Add RGB protocol for B-family
 10/19/12   llg     (Tech Pubs) Edited/added Doxygen comments and markup.
 08/09/12   sm      New file
=============================================================================*/
#ifndef __EFIPMICRGBLED_H__
#define __EFIPMICRGBLED_H__

/** @ingroup efi_pmicRgbLed_constants
  Protocol version.
*/
#define PMIC_RGB_LED_REVISION 0x0000000000010002

/*  Protocol GUID definition */
/** @ingroup efi_pmicGpio_protocol */
#define EFI_PMIC_RGB_LED_PROTOCOL_GUID \
  { 0x3ccead8e, 0xb24c, 0x403b, { 0xa0, 0xb2, 0xfd, 0x4c, 0x56, 0xb4, 0x6c, 0x9f } }

/* PMIC UEFI typedefs */
/** @cond */
typedef struct _EFI_QCOM_PMIC_RGB_LED_PROTOCOL  EFI_QCOM_PMIC_RGB_LED_PROTOCOL;
/** @endcond */

/** @ingroup efi_pmicRgbLed_protocol */
extern EFI_GUID gQcomPmicRgbLedProtocolGuid;

/*===========================================================================
  TYPE DEFINITIONS
===========================================================================*/

/** @addtogroup efi_pmicRgbLed_constants 
@{ */
/** Blue color. */
#define EFI_PM_RGB_LED_BLUE     0x01
/** Green color. */
#define EFI_PM_RGB_LED_GREEN    0x02
/** Red color. */
#define EFI_PM_RGB_LED_RED      0x04

#define EFI_PM_RGB_SEGMENT_ALL   0x7

/* 
 * LED brightness level
 */
#define EFI_PM_RGB_DIM_LEVEL_LOW  0x008
#define EFI_PM_RGB_DIM_LEVEL_MID  0x016
#define EFI_PM_RGB_DIM_LEVEL_HIGH 0x018
#define EFI_PM_RGB_DIM_LEVEL_MAX  0x1FF


/** @} */ /* end_addtogroup efi_pmicRgbLed_constants */

/*! \enum EFI_PM_RGB_WHICH_TYPE
 *  \brief Type for selecting the available RGB peripherals.
 */
typedef enum
{
  EFI_PM_RGB_1,       /**< RGB1. */
  EFI_PM_RGB_INVALID  /**< INVALID RGB. */
} EFI_PM_RGB_WHICH_TYPE;



/*! \enum EFI_PM_RGB_STATUS_TYPE
 *  \brief Structure for RGB status.
 */
typedef struct 
{
  UINT32                          EnabledRgbMask;
} EFI_PM_RGB_STATUS_TYPE;

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
/* EFI_PMIC_RGB_LED_ENABLE_LED_DRIVER */ 
/** @ingroup efi_pmicRgbLed_enable_led_driver
  @par Summary
  Enables LEDs of specified colors. For example, to select multiple 
  colors, blue and red:

  @code
    EnableLedDriver(PM_UEFI_RGB_LED_BLUE | PM_UEFI_RGB_LED_RED)
  @endcode
 
  @param[in]  PmicDeviceIndex    Primary: 0 Secondary:1
  @param[in]  Rgb                RGB periphearl type. Refer enum EFI_PM_RGB_WHICH_TYPE
  @param[in]  Color              Color of the LED to be enabled by the ATC.
                                 e.g. EFI_PM_RGB_LED_NONE | EFI_PM_RGB_LED_BLUE | EFI_PM_RGB_LED_RED
  @param[in]  Enable             TRUE enables and FALSE disables

  @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
  EFI_NOT_READY         -- Physical device is busy or not ready to
                           process this request.
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_RGB_LED_ENABLE_LED_DRIVER)(
  IN UINT32                          PmicDeviceIndex,
  IN EFI_PM_RGB_WHICH_TYPE           Rgb,
  IN UINT8                           Color,
  IN BOOLEAN                         Enable
);


/* EFI_PMIC_RGB_LED_GET_STATUS */
/** @ingroup efi_pmicRgbLed_get_status
  @par Summary
  sets the voltage source for the given RGB peripheral.

  @param[IN] PmicDeviceIndex     For Primary   :0                 
                                 For Secondary :1
  @param[IN] Rgb                 RGB peripheral type ; see #EFI_PM_RGB_WHICH_TYPE for 
                                 details.
  @param[OUT] Status             RGB status; see #EFI_PM_RGB_STATUS_TYPE for 
                                 details.

 @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- Parameter is invalid. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
  EFI_NOT_READY         -- Physical device is busy or not ready to
                           process this request.
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_RGB_LED_GET_STATUS)(
  IN  UINT32                          PmicDeviceIndex,
  IN  EFI_PM_RGB_WHICH_TYPE           Rgb,
  OUT EFI_PM_RGB_STATUS_TYPE          *RgbStatus
);

/* EFI_PMIC_RGB_LED_GET_STATUS */
/** @ingroup efi_pmicRgbLed_get_status
  @par Summary
  sets the voltage source for the given RGB peripheral.

  @param[IN] PmicDeviceIndex     For Primary   :0                 
                                 For Secondary :1
  @param[IN] Rgb                 RGB peripheral type ; see #EFI_PM_RGB_WHICH_TYPE for 
                                 details.
  @param[IN] rgb_mask            RGB Mask ; see #EFI_PM_RGB_STATUS_TYPE for 
                                 details.
 @param[IN] source_type          source type ; see #EFI_PM_RGB_VOLTAGE_SOURCE_TYPE for 
                                 details.
 @param[IN] dim_level            Dim level;
 @param[IN] enable_rgb           Enable or Disable RGB LED ;

 @return 
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- Parameter is invalid. \n
  EFI_DEVICE_ERROR      -- Physical device reported an error. \n
  EFI_NOT_READY         -- Physical device is busy or not ready to
                           process this request.
*/
typedef
EFI_STATUS (EFIAPI *EFI_PMIC_RGB_LED_ENABLE_RGB_LED)(
  IN  UINT32                          PmicDeviceIndex,
  IN  EFI_PM_RGB_WHICH_TYPE           Rgb,
  IN  UINT32                          rgb_mask,
  IN  UINT32                          dim_level,
  IN  BOOLEAN                         enable_rgb
);

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_pmicRgbLed_protocol
  @par Summary
  Qualcomm PMIC RGB LED Protocol interface.

  @par Parameters
  @inputprotoparams{pmic_rgb_led_proto_params.tex} 
*/
struct _EFI_QCOM_PMIC_RGB_LED_PROTOCOL {
  UINT64                               Revision;
  EFI_PMIC_RGB_LED_ENABLE_LED_DRIVER   EnableLedDriver;
  EFI_PMIC_RGB_LED_GET_STATUS          GetStatus;
  EFI_PMIC_RGB_LED_ENABLE_RGB_LED      ConfigRgbLed;
};

#endif  /* __EFIPMICRGBLED_H__ */
