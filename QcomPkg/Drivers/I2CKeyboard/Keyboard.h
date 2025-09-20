/** @file
  Function prototype for I2C Keyboard Driver.

Copyright (c) 2018, Qualcomm Technologies Inc. All rights reserved.
Portions Copyright (c) 2004 - 2013, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

/*=============================================================================
                              EDIT HISTORY


 when       who      what, where, why
 --------   ---      ----------------------------------------------------------
 12/11/18   ssumit  Header file renamed as per I2C Core implementation
 03/13/17   ltg     Initial revision for UEFI I2C Keyboard Driver 
=============================================================================*/

#ifndef _EFI_KEYBOARD_H_
#define _EFI_KEYBOARD_H_


#include "EfiKey.h"
#include "i2c_api.h"

#define TABLE_END 0x0
#define KEYBOARD_TIMER_INTERVAL    200000  // 0.02s
#define SCANCODE_CAPS_LOCK         0x3A

/**
  Converts Keycode ranging from 0x4 to 0x65 to EFI_INPUT_KEY.

  @param  I2CKeyboardDevice     The I2C_KB_DEV instance.
  @param  KeyCode               Indicates the key code that will be interpreted.
  @param  KeyData               A pointer to a buffer that is filled in with
                                the keystroke information for the key that
                                was pressed.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER KeyCode is not in the range of 0x4 to 0x65.
  @retval EFI_INVALID_PARAMETER Translated EFI_INPUT_KEY has zero for both ScanCode and UnicodeChar.
  @retval EFI_NOT_READY         KeyCode represents a dead key.
  @retval EFI_DEVICE_ERROR      Keyboard is invalid.

**/
EFI_STATUS
KeyCodeToEfiInputKey (
  IN  I2C_KB_DEV                *I2CKeyboardDevice,
  IN  UINT8                     KeyCode,
  OUT EFI_KEY_DATA              *KeyData
  );

#endif
