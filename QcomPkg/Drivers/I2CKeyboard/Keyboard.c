/** @file
  Helper functions for I2C Keyboard Driver.

Copyright (c) 2017,2022 Qualcomm Technologies Inc. All rights reserved.
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
 09/08/22   ananns  Added SetKeyboardLED function call for CAP's lock LED functionality
 05/05/17   ltg     Added Home key to struct 
 03/13/17   ltg     Initial revision for UEFI I2C Keyboard Driver 
=============================================================================*/

#include "Keyboard.h"

struct {
  UINT8   KeyCode;             ///< follows value defined in Scan Code Set1
  UINT16  ScanCode;
  CHAR16  UnicodeChar;
  CHAR16  ShiftUnicodeChar;
}
ConvertKeyboardKeyCodeToEfiKey[] = {
  {
    0x29,  //   Escape
    SCAN_ESC,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x1E,
    SCAN_NULL,
    L'1',
    L'!'
  },
  {
    0x1F,
    SCAN_NULL,
    L'2',
    L'@'
  },
  {
    0x20,
    SCAN_NULL,
    L'3',
    L'#'
  },
  {
    0x21,
    SCAN_NULL,
    L'4',
    L'$'
  },
  {
    0x22,
    SCAN_NULL,
    L'5',
    L'%'
  },
  {
    0x23,
    SCAN_NULL,
    L'6',
    L'^'
  },
  {
    0x24,
    SCAN_NULL,
    L'7',
    L'&'
  },
  {
    0x25,
    SCAN_NULL,
    L'8',
    L'*'
  },
  {
    0x26,
    SCAN_NULL,
    L'9',
    L'('
  },
  {
    0x27,
    SCAN_NULL,
    L'0',
    L')'
  },
  {
    0x2D,
    SCAN_NULL,
    L'-',
    L'_'
  },
  {
    0x2E,
    SCAN_NULL,
    L'=',
    L'+'
  },
  {
    0x2A, //  BackSpace
    SCAN_NULL,
    CHAR_BACKSPACE,
    CHAR_BACKSPACE
  },
  {
    0x2B, //  Tab
    SCAN_NULL,
    CHAR_TAB,
    CHAR_TAB
  },
  {
    0x14,
    SCAN_NULL,
    L'q',
    L'Q'
  },
  {
    0x1A,
    SCAN_NULL,
    L'w',
    L'W'
  },
  {
    0x08,
    SCAN_NULL,
    L'e',
    L'E'
  },
  {
    0x15,
    SCAN_NULL,
    L'r',
    L'R'
  },
  {
    0x17,
    SCAN_NULL,
    L't',
    L'T'
  },
  {
    0x1C,
    SCAN_NULL,
    L'y',
    L'Y'
  },
  {
    0x18,
    SCAN_NULL,
    L'u',
    L'U'
  },
  {
    0x0C,
    SCAN_NULL,
    L'i',
    L'I'
  },
  {
    0x12,
    SCAN_NULL,
    L'o',
    L'O'
  },
  {
    0x13,
    SCAN_NULL,
    L'p',
    L'P'
  },
  {
    0x2F,
    SCAN_NULL,
    L'[',
    L'{'
  },
  {
    0x30,
    SCAN_NULL,
    L']',
    L'}'
  },
  {
    0x28, //   Enter
    SCAN_NULL,
    CHAR_CARRIAGE_RETURN,
    CHAR_CARRIAGE_RETURN
  },
  {
    0x04,
    SCAN_NULL,
    L'a',
    L'A'
  },
  {
    0x16,
    SCAN_NULL,
    L's',
    L'S'
  },
  {
    0x07,
    SCAN_NULL,
    L'd',
    L'D'
  },
  {
    0x09,
    SCAN_NULL,
    L'f',
    L'F'
  },
  {
    0x0A,
    SCAN_NULL,
    L'g',
    L'G'
  },
  {
    0x0B,
    SCAN_NULL,
    L'h',
    L'H'
  },
  {
    0x0D,
    SCAN_NULL,
    L'j',
    L'J'
  },
  {
    0x0E,
    SCAN_NULL,
    L'k',
    L'K'
  },
  {
    0x0F,
    SCAN_NULL,
    L'l',
    L'L'
  },
  {
    0x33,
    SCAN_NULL,
    L';',
    L':'
  },
  {
    0x34,
    SCAN_NULL,
    L'\'',
    L'"'
  },
  {
    0x35,
    SCAN_NULL,
    L'`',
    L'~'
  },
  {
    0x31,
    SCAN_NULL,
    L'\\',
    L'|'
  },
  {
    0x1D,
    SCAN_NULL,
    L'z',
    L'Z'
  },
  {
    0x1B,
    SCAN_NULL,
    L'x',
    L'X'
  },
  {
    0x06,
    SCAN_NULL,
    L'c',
    L'C'
  },
  {
    0x19,
    SCAN_NULL,
    L'v',
    L'V'
  },
  {
    0x05,
    SCAN_NULL,
    L'b',
    L'B'
  },
  {
    0x11,
    SCAN_NULL,
    L'n',
    L'N'
  },
  {
    0x10,
    SCAN_NULL,
    L'm',
    L'M'
  },
  {
    0x36,
    SCAN_NULL,
    L',',
    L'<'
  },
  {
    0x37,
    SCAN_NULL,
    L'.',
    L'>'
  },
  {
    0x38,
    SCAN_NULL,
    L'/',
    L'?'
  },
  {
    0x2C,
    SCAN_NULL,
    L' ',
    L' '
  },
  {
    0x39, //CapsLock
    SCANCODE_CAPS_LOCK,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x3A,
    SCAN_F1,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x3B,
    SCAN_F2,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x3C,
    SCAN_F3,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x3D,
    SCAN_F4,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x3E,
    SCAN_F5,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x3F,
    SCAN_F6,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x40,
    SCAN_F7,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x41,
    SCAN_F8,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x42,
    SCAN_F9,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x43,
    SCAN_F10,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x44,
    SCAN_F11,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x45,
    SCAN_F12,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x46,  //Print Screen
    SCAN_NULL,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x4A,  //Home
    SCAN_HOME,
    CHAR_NULL,
    CHAR_NULL
  },  
  {
    0x4C,  //delete
    SCAN_DELETE,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x4F,  //right arrow
    SCAN_RIGHT,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x50,  //left arrow
    SCAN_LEFT,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x51,  //down arrow
    SCAN_DOWN,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    0x52,  //up arrow
    SCAN_UP,
    CHAR_NULL,
    CHAR_NULL
  },
  {
    TABLE_END,
    SCAN_NULL,
    CHAR_NULL,
    CHAR_NULL
  },
};

/**
  Converts USB Keycode ranging from 0x4 to 0x65 to EFI_INPUT_KEY.

  @param  I2CKeyboardDevice     The USB_KB_DEV instance.
  @param  KeyCode               Indicates the key code that will be interpreted.
  @param  KeyData               A pointer to a buffer that is filled in with
                                the keystroke information for the key that
                                was pressed.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER KeyCode is not in the range of 0x4 to 0x65.
  @retval EFI_INVALID_PARAMETER Translated EFI_INPUT_KEY has zero for both ScanCode and UnicodeChar.
  @retval EFI_NOT_READY         KeyCode represents a dead key with EFI_NS_KEY_MODIFIER
  @retval EFI_DEVICE_ERROR      Keyboard layout is invalid.

**/
EFI_STATUS
KeyCodeToEfiInputKey (
  IN  I2C_KB_DEV                *I2CKeyboardDevice,
  IN  UINT8                     KeyCode,
  OUT EFI_KEY_DATA              *KeyData
  )
{
  LIST_ENTRY                     *Link;
  KEYBOARD_CONSOLE_IN_EX_NOTIFY  *CurrentNotify;
  UINTN                          Index;
  EFI_STATUS                     KbLedStatus = EFI_SUCCESS;

  KeyData->Key.ScanCode = SCAN_NULL;
  KeyData->Key.UnicodeChar = CHAR_NULL;

  for (Index = 0; ConvertKeyboardKeyCodeToEfiKey[Index].KeyCode != TABLE_END; Index++) {
      if (KeyCode == ConvertKeyboardKeyCodeToEfiKey[Index].KeyCode) {
        KeyData->Key.ScanCode    = ConvertKeyboardKeyCodeToEfiKey[Index].ScanCode;
        KeyData->Key.UnicodeChar = ConvertKeyboardKeyCodeToEfiKey[Index].UnicodeChar;

        if ((I2CKeyboardDevice->LeftShift || I2CKeyboardDevice->RightShift) &&
            (ConvertKeyboardKeyCodeToEfiKey[Index].UnicodeChar != ConvertKeyboardKeyCodeToEfiKey[Index].ShiftUnicodeChar)) {
          KeyData->Key.UnicodeChar = ConvertKeyboardKeyCodeToEfiKey[Index].ShiftUnicodeChar;
          //
          // Need not return associated shift state if a class of printable characters that
          // are normally adjusted by shift modifiers. e.g. Shift Key + 'f' key = 'F'
          //
          KeyData->KeyState.KeyShiftState &= ~(EFI_LEFT_SHIFT_PRESSED | EFI_RIGHT_SHIFT_PRESSED);
        }
        //
        // alphabetic key is affected by CapsLock State
        //
        if (I2CKeyboardDevice->CapsLock) {
          if (KeyData->Key.UnicodeChar >= L'a' && KeyData->Key.UnicodeChar <= L'z') {
            KeyData->Key.UnicodeChar = (UINT16) (KeyData->Key.UnicodeChar - L'a' + L'A');
          } else if (KeyData->Key.UnicodeChar >= L'A' && KeyData->Key.UnicodeChar <= L'Z') {
            KeyData->Key.UnicodeChar = (UINT16) (KeyData->Key.UnicodeChar - L'A' + L'a');
          }
        }
        break;
      }
    }  

  //
  // If the key can not be converted then just return.
  //
  if (KeyData->Key.ScanCode == SCAN_NULL && KeyData->Key.UnicodeChar == CHAR_NULL) {
    if (!I2CKeyboardDevice->IsSupportPartialKey) {
      return EFI_NOT_READY;
    }
  }

  //
  // Check for special keys and update the driver state.
  //
  if (KeyData->Key.ScanCode == SCANCODE_CAPS_LOCK){
    I2CKeyboardDevice->CapsLock = (BOOLEAN)!I2CKeyboardDevice->CapsLock;
    
    //
    // Enable the KB CAP's Lock LED
    //
    KbLedStatus = SetKeyboardLED (I2CKeyboardDevice);
    if ( KbLedStatus != EFI_SUCCESS){
      DEBUG((EFI_D_WARN, "[I2CKeyboard] SetKeyboardLED Failed : %r\n",KbLedStatus));
    }
  }

  //
  // Save Shift/Toggle state
  //
  KeyData->KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID;
  KeyData->KeyState.KeyToggleState = EFI_TOGGLE_STATE_VALID;


  if (I2CKeyboardDevice->LeftCtrl) {
    KeyData->KeyState.KeyShiftState |= EFI_LEFT_CONTROL_PRESSED;
  }
  if (I2CKeyboardDevice->RightCtrl) {
    KeyData->KeyState.KeyShiftState |= EFI_RIGHT_CONTROL_PRESSED;
  }
  if (I2CKeyboardDevice->LeftAlt) {
    KeyData->KeyState.KeyShiftState |= EFI_LEFT_ALT_PRESSED;
  }
  if (I2CKeyboardDevice->RightAlt) {
    KeyData->KeyState.KeyShiftState |= EFI_RIGHT_ALT_PRESSED;
  }
  if (I2CKeyboardDevice->LeftShift) {
    KeyData->KeyState.KeyShiftState |= EFI_LEFT_SHIFT_PRESSED;
  }
  if (I2CKeyboardDevice->RightShift) {
    KeyData->KeyState.KeyShiftState |= EFI_RIGHT_SHIFT_PRESSED;
  }
  if (I2CKeyboardDevice->LeftLogo) {
    KeyData->KeyState.KeyShiftState |= EFI_LEFT_LOGO_PRESSED;
  }
  if (I2CKeyboardDevice->RightLogo) {
    KeyData->KeyState.KeyShiftState |= EFI_RIGHT_LOGO_PRESSED;
  }
  if (I2CKeyboardDevice->Menu) {
    KeyData->KeyState.KeyShiftState |= EFI_MENU_KEY_PRESSED;
  }
  if (I2CKeyboardDevice->SysReq) {
    KeyData->KeyState.KeyShiftState |= EFI_SYS_REQ_PRESSED;
  }

  if (I2CKeyboardDevice->ScrollLock) {
    KeyData->KeyState.KeyToggleState |= EFI_SCROLL_LOCK_ACTIVE;
  }
  if (I2CKeyboardDevice->NumLock) {
    KeyData->KeyState.KeyToggleState |= EFI_NUM_LOCK_ACTIVE;
  }
  if (I2CKeyboardDevice->CapsLock) {
    KeyData->KeyState.KeyToggleState |= EFI_CAPS_LOCK_ACTIVE;
  }
  if (I2CKeyboardDevice->IsSupportPartialKey) {
    KeyData->KeyState.KeyToggleState |= EFI_KEY_STATE_EXPOSED;
  }

  //
  // Invoke notification functions if exist
  //
  for (Link = GetFirstNode (&I2CKeyboardDevice->NotifyList); !IsNull (&I2CKeyboardDevice->NotifyList, Link); Link = GetNextNode (&I2CKeyboardDevice->NotifyList, Link)) {
    CurrentNotify = CR (
                      Link,
                      KEYBOARD_CONSOLE_IN_EX_NOTIFY,
                      NotifyEntry,
                      I2C_KB_CONSOLE_IN_EX_NOTIFY_SIGNATURE
                      );
    if (IsKeyRegistered (&CurrentNotify->KeyData, KeyData)) {
      DEBUG((EFI_D_INFO, "[I2CKeyboard] Calling notification function\n"));
      CurrentNotify->KeyNotificationFn (KeyData);
    }
  }

  return EFI_SUCCESS;
}