/** @file
  Header file for I2C Keyboard Driver's Data Structures.

Copyright (c) 2017-2018,2022 Qualcomm Technologies, Inc.  All Rights Reserved.
Portions Copyright (c) 2004 - 2012, Intel Corporation. All rights reserved.<BR>
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
 09/08/22   ananns  Added LED_MAP structure for CAP's lock LED functinality
 03/13/17   ssumit  Changes in I2C structure and Slave Feq & address as per Primus configuration
 03/13/17   ltg     Initial revision for UEFI I2C Keyboard Driver 
=============================================================================*/


#ifndef _EFI_I2C_KB_H_
#define _EFI_I2C_KB_H_


#include <Uefi.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextInEx.h>

#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include "i2c_api.h"

#define HID_DESC_LENGTH 30
#define TRANSFER_DELAY 50

#define I2C_KB_DEV_SIGNATURE  SIGNATURE_32 ('i', 'k', 'b', 'd')
#define I2C_KB_CONSOLE_IN_EX_NOTIFY_SIGNATURE SIGNATURE_32 ('i', 'k', 'b', 'x')

// Get High Bytes
#define HIGH_BYTE(WORD16) (((WORD16) & 0xFF00) >> 8)

// Get Low Bytes
#define LOW_BYTE(WORD16)  (((WORD16) & 0x00FF))

// I2C INFO
#define I2C_DEFAULT_BUS_FREQ 400
#define I2C_SLAVE_MAX_CLOCK_STRETCH_US 500

// I2C keyboard Slave address 
#define I2C_SLAVE_ADDR 0x003A

// HID DESC ADDR
#define HID_DESC_ADDR_HIGH 0x00
#define HID_DESC_ADDR_LOW 0x01

// COMMANDS
#define RESET_CMD_HIGH 0x01
#define RESET_CMD_LOW 0x00
#define ENABLE_CMD_HIGH 0x08
#define ENABLE_CMD_LOW 0x00


//--------------------------------------------------------------------------------
// Keyboard HidDescriptor
//--------------------------------------------------------------------------------
typedef struct
{
  UINT16 wHidDescLength;
  UINT16 bcdVersion;
  UINT16 wReportDescLength;
  UINT16 wReportDescRegister;
  UINT16 wInputRegister;
  UINT16 wMaxInputLength;
  UINT16 wOutputRegister;
  UINT16 wMaxOutputLength;
  UINT16 wCommandRegister;
  UINT16 wDataRegister;
  UINT16 wVendorID;
  UINT16 wProductID;
  UINT16 wVersionID;
  UINT32 Reserved;
} HidDescriptor;

//--------------------------------------------------------------------------------
// Keyboard InputReport
//--------------------------------------------------------------------------------
typedef struct
{
  UINT16 length;
  UINT8  reportID;                      // Report ID // Collection: Keyboard
  UINT8  KeyboardLeftControl : 1;       // Usage 0x000700E0: Keyboard Left Control, Value = 0 to 1
  UINT8  KeyboardLeftShift : 1;         // Usage 0x000700E1: Keyboard Left Shift, Value = 0 to 1
  UINT8  KeyboardLeftAlt : 1;           // Usage 0x000700E2: Keyboard Left Alt, Value = 0 to 1
  UINT8  KeyboardLeftGui : 1;           // Usage 0x000700E3: Keyboard Left GUI, Value = 0 to 1
  UINT8  KeyboardRightControl : 1;      // Usage 0x000700E4: Keyboard Right Control, Value = 0 to 1
  UINT8  KeyboardRightShift : 1;        // Usage 0x000700E5: Keyboard Right Shift, Value = 0 to 1
  UINT8  KeyboardRightAlt : 1;          // Usage 0x000700E6: Keyboard Right Alt, Value = 0 to 1
  UINT8  KeyboardRightGui : 1;          // Usage 0x000700E7: Keyboard Right GUI, Value = 0 to 1
  UINT8  pad_2;                         // Pad
  UINT8  Keyboard_data[6];              // Value = 0 to 255
} InputReport;

typedef struct {
  UINT8 KeyboardNumLock : 1;
  UINT8 KeyboardCapsLock : 1;
  UINT8 KeyboardScrollLock : 1;
  UINT8 Resrvd : 5;
} KEYBOARD_LED_MAP;

typedef struct _KEYBOARD_CONSOLE_IN_EX_NOTIFY {
  UINTN                               Signature;
  EFI_KEY_DATA                        KeyData;
  EFI_KEY_NOTIFY_FUNCTION             KeyNotificationFn;
  LIST_ENTRY                          NotifyEntry;
} KEYBOARD_CONSOLE_IN_EX_NOTIFY;

#define KEYBOARD_KEY_CODE_MAX_COUNT  32
typedef struct {
  UINT8                               Buffer[KEYBOARD_KEY_CODE_MAX_COUNT];
  UINTN                               Head;
  UINTN                               Tail;
} KEY_CODE_QUEUE;

#define KEYBOARD_EFI_KEY_MAX_COUNT    256
typedef struct {
  EFI_KEY_DATA                        Buffer[KEYBOARD_EFI_KEY_MAX_COUNT];
  UINTN                               Head;
  UINTN                               Tail;
} EFI_KEY_QUEUE;

///
/// Structure to describe I2C keyboard device
///
typedef struct {
  UINTN                               Signature;
  UINTN                               ReportID;

  EFI_SIMPLE_TEXT_INPUT_PROTOCOL      SimpleInput;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL   SimpleInputEx;

  void *                              I2CHandle;
  i2c_slave_config                          I2CConfig;

  EFI_EVENT                           TimerEvent;

  UINT8                               LastKeyCodeArray[6];

  BOOLEAN                             LeftCtrl;
  BOOLEAN                             RightCtrl;
  BOOLEAN                             LeftAlt;
  BOOLEAN                             RightAlt;
  BOOLEAN                             LeftShift;
  BOOLEAN                             RightShift;
  BOOLEAN                             LeftLogo;
  BOOLEAN                             RightLogo;
  BOOLEAN                             Menu;
  BOOLEAN                             SysReq;
  BOOLEAN                             CapsLock;
  BOOLEAN                             NumLock;
  BOOLEAN                             ScrollLock;

  BOOLEAN                             IsSupportPartialKey;

  KEY_CODE_QUEUE                      KeyCodeQueue;
  EFI_KEY_QUEUE                       EfiKeyQueue;

  //
  // Notification Function List
  //
  LIST_ENTRY                          NotifyList;
} I2C_KB_DEV;

#define I2C_KB_DEV_FROM_THIS(a) \
    CR(a, I2C_KB_DEV, SimpleInput, I2C_KB_DEV_SIGNATURE)
#define TEXT_INPUT_EX_I2C_KB_DEV_FROM_THIS(a) \
    CR(a, I2C_KB_DEV, SimpleInputEx, I2C_KB_DEV_SIGNATURE)

//
// According to Universal Serial Bus HID Usage Tables document ver 1.12,
// a Boot Keyboard should support the keycode range from 0x0 to 0x65 and 0xE0 to 0xE7.
// 0xE0 to 0xE7 are for modifier keys, and 0x0 to 0x3 are reserved for typical
// keyboard status or keyboard errors.
// So the number of valid non-modifier USB keycodes is 0x62, and the number of
// valid keycodes is 0x6A.
//
#define NUMBER_OF_VALID_NON_MODIFIER_I2C_KEYCODE      0x62
#define NUMBER_OF_VALID_I2C_KEYCODE                   0x6A
//
// 0x0 to 0x3 are reserved for typical keyboard status or keyboard errors.
//
#define I2CKBD_VALID_KEYCODE(Key) ((UINT8) (Key) > 3)


//
// HID functions
//
/**
  Reset the device

  @param i2c_handle     Pointer to i2c handle
  @param cfg            Pointer to i2c config

  @retval EFI_SUCCESS             Reset completed successfully.
  @retval EFI_INVALID_PARAMETER   I2C handle is not confiured properly.
  @retval EFI_DEVICE_ERROR        Device did not respond.
**/
EFI_STATUS 
ResetDevice(
  IN VOID * i2c_handle, IN i2c_slave_config cfg
  );

/**
  Get the input report from the device

  @param i2c_handle     Pointer to i2c handle
  @param cfg            Pointer to i2c config

  @retval EFI_SUCCESS             Input report retrieval completed successfully.
  @retval EFI_INVALID_PARAMETER   I2C handle is not confiured properly.
  @retval EFI_DEVICE_ERROR        Device did not respond.
**/
EFI_STATUS 
GetInputReport(
  IN VOID * i2c_handle, IN i2c_slave_config cfg, OUT InputReport* gReportDesc
  );

/**
  Get report descriptor from HID compliant device

  @param i2c_handle     Pointer to i2c handle
  @param cfg            Pointer to i2c config

  @retval EFI_SUCCESS             Report descriptor retrieved successfully.
  @retval EFI_INVALID_PARAMETER   I2C handle is not confiured properly.
  @retval EFI_DEVICE_ERROR        Device did not respond.
**/
EFI_STATUS 
ReadReportDescriptor(
  IN VOID * i2c_handle, IN i2c_slave_config cfg, OUT UINT8* ReportDesc
  );

/**
  Parse report descriptor from HID compliant device

  @param ReportDesc     Pointer to report descriptor

  @retval EFI_SUCCESS             Report descriptor parsed successfully.
  @retval EFI_DEVICE_ERROR        Device is not valid.
**/
EFI_STATUS
ParseReportDescriptor( 
  IN UINT8 * ReportDesc, OUT UINT8 * ReportID
  );

/**
  Get HID descriptor from HID compliant device

  @param i2c_handle     Pointer to i2c handle
  @param cfg            Pointer to i2c config

  @retval EFI_SUCCESS             HID descriptor retrieved successfully.
  @retval EFI_INVALID_PARAMETER   I2C handle is not confiured properly.
  @retval EFI_DEVICE_ERROR        Device did not respond.
**/
EFI_STATUS
ReadHidDescriptor(
  IN VOID * i2c_handle, IN i2c_slave_config cfg
  );

//
// Queue functions
//
/**
  Check whether the EFI key buffer is empty.

  @param Queue     Pointer to instance of EFI_KEY_QUEUE.

  @retval TRUE    The EFI key buffer is empty.
  @retval FALSE   The EFI key buffer isn't empty.
**/
BOOLEAN
IsEfikeyBufEmpty (
  IN  EFI_KEY_QUEUE         *Queue
  );


/**
  Read & remove one key data from the EFI key buffer.

  @param Queue     Pointer to instance of EFI_KEY_QUEUE.
  @param KeyData   Receive the key data.

  @retval EFI_SUCCESS   The key data is popped successfully.
  @retval EFI_NOT_READY There is no key data available.
**/
EFI_STATUS
PopEfikeyBufHead (
  IN  EFI_KEY_QUEUE         *Queue,
  OUT EFI_KEY_DATA          *KeyData OPTIONAL
  );

/**
  Push one key data to the EFI key buffer.

  @param Queue     Pointer to instance of EFI_KEY_QUEUE.
  @param KeyData   The key data to push.
**/
VOID
PushEfikeyBufTail (
  IN  EFI_KEY_QUEUE         *Queue,
  IN  EFI_KEY_DATA          *KeyData
  );

//
// Functions of Simple Text Input Protocol and Simple Text Input Protocol Ex
//
/**
  Reset the input device

  There are 2 types of reset for I2C keyboard.
  For non-exhaustive reset, only keyboard buffer is cleared.
  For exhaustive reset, in addition to clearance of keyboard buffer, the hardware status
  is also re-initialized.

  @param  This                 Protocol instance pointer.
  @param  ExtendedVerification Driver may perform diagnostics on reset.

  @retval EFI_SUCCESS          The device was reset.
  @retval EFI_DEVICE_ERROR     The device is not functioning properly and could not be reset.

**/
EFI_STATUS
EFIAPI
I2CKeyboardReset (
  IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL   *This,
  IN  BOOLEAN                          ExtendedVerification
  );

/**
  Reads the next keystroke from the input device.

  @param  This                 The EFI_SIMPLE_TEXT_INPUT_PROTOCOL instance.
  @param  Key                  A pointer to a buffer that is filled in with the keystroke
                               information for the key that was pressed.

  @retval EFI_SUCCESS          The keystroke information was returned.
  @retval EFI_NOT_READY        There was no keystroke data availiable.
  @retval EFI_DEVICE_ERROR     The keydtroke information was not returned due to
                               hardware errors.

**/
EFI_STATUS
EFIAPI
I2CKeyboardReadKeyStroke (
  IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL   *This,
  OUT EFI_INPUT_KEY                    *Key
  );

/**
  Resets the input device hardware.

  The Reset() function resets the input device hardware. As part
  of initialization process, the firmware/device will make a quick
  but reasonable attempt to verify that the device is functioning.
  If the ExtendedVerification flag is TRUE the firmware may take
  an extended amount of time to verify the device is operating on
  reset. Otherwise the reset operation is to occur as quickly as
  possible. The hardware verification process is not defined by
  this specification and is left up to the platform firmware or
  driver to implement.

  @param This                 A pointer to the EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL instance.

  @param ExtendedVerification Indicates that the driver may perform a more exhaustive
                              verification operation of the device during reset.

  @retval EFI_SUCCESS         The device was reset.
  @retval EFI_DEVICE_ERROR    The device is not functioning correctly and could not be reset.

**/
EFI_STATUS
EFIAPI
I2CKeyboardResetEx (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *This,
  IN BOOLEAN                            ExtendedVerification
  );

/**
  Reads the next keystroke from the input device.

  @param  This                   Protocol instance pointer.
  @param  KeyData                A pointer to a buffer that is filled in with the keystroke
                                 state data for the key that was pressed.

  @retval EFI_SUCCESS            The keystroke information was returned.
  @retval EFI_NOT_READY          There was no keystroke data available.
  @retval EFI_DEVICE_ERROR       The keystroke information was not returned due to
                                 hardware errors.
  @retval EFI_INVALID_PARAMETER  KeyData is NULL.

**/
EFI_STATUS
EFIAPI
I2CKeyboardReadKeyStrokeEx (
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  OUT EFI_KEY_DATA                      *KeyData
  );

/**
  Set certain state for the input device.

  @param  This                    Protocol instance pointer.
  @param  KeyToggleState          A pointer to the EFI_KEY_TOGGLE_STATE to set the
                                  state for the input device.

  @retval EFI_SUCCESS             The device state was set appropriately.
  @retval EFI_DEVICE_ERROR        The device is not functioning correctly and could
                                  not have the setting adjusted.
  @retval EFI_UNSUPPORTED         The device does not support the ability to have its state set.
  @retval EFI_INVALID_PARAMETER   KeyToggleState is NULL.

**/
EFI_STATUS
EFIAPI
I2CKeyboardSetState (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *This,
  IN EFI_KEY_TOGGLE_STATE               *KeyToggleState
  );

/**
  Register a notification function for a particular keystroke for the input device.

  @param  This                        Protocol instance pointer.
  @param  KeyData                     A pointer to a buffer that is filled in with the keystroke
                                      information data for the key that was pressed.
  @param  KeyNotificationFunction     Points to the function to be called when the key
                                      sequence is typed specified by KeyData.
  @param  NotifyHandle                Points to the unique handle assigned to the registered notification.

  @retval EFI_SUCCESS                 The notification function was registered successfully.
  @retval EFI_OUT_OF_RESOURCES        Unable to allocate resources for necesssary data structures.
  @retval EFI_INVALID_PARAMETER       KeyData or NotifyHandle or KeyNotificationFunction is NULL.

**/
EFI_STATUS
EFIAPI
I2CKeyboardRegisterKeyNotify (
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *This,
  IN  EFI_KEY_DATA                       *KeyData,
  IN  EFI_KEY_NOTIFY_FUNCTION            KeyNotificationFunction,
  OUT VOID                               **NotifyHandle
  );

/**
  Remove a registered notification function from a particular keystroke.

  @param  This                      Protocol instance pointer.
  @param  NotificationHandle        The handle of the notification function being unregistered.

  @retval EFI_SUCCESS              The notification function was unregistered successfully.
  @retval EFI_INVALID_PARAMETER    The NotificationHandle is invalid
  @retval EFI_NOT_FOUND            Cannot find the matching entry in database.

**/
EFI_STATUS
EFIAPI
I2CKeyboardUnregisterKeyNotify (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *This,
  IN VOID                               *NotificationHandle
  );

/**
  Event notification function registered for EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL.WaitForKeyEx
  and EFI_SIMPLE_TEXT_INPUT_PROTOCOL.WaitForKey.

  @param  Event        Event to be signaled when a key is pressed.
  @param  Context      Points to I2C_KB_DEV instance.

**/
VOID
EFIAPI
I2CKeyboardWaitForKey (
  IN  EFI_EVENT               Event,
  IN  VOID                    *Context
  );

/**
  Free keyboard notify list.

  @param  NotifyList              The keyboard notify list to free.

  @retval EFI_SUCCESS             Free the notify list successfully.
  @retval EFI_INVALID_PARAMETER   NotifyList is NULL.

**/
EFI_STATUS
KbdFreeNotifyList (
  IN OUT LIST_ENTRY           *NotifyList
  );

/**
  Check whether the pressed key matches a registered key or not.

  @param  RegsiteredData    A pointer to keystroke data for the key that was registered.
  @param  InputData         A pointer to keystroke data for the key that was pressed.

  @retval TRUE              Key pressed matches a registered key.
  @retval FLASE             Key pressed does not matche a registered key.

**/
BOOLEAN
IsKeyRegistered (
  IN EFI_KEY_DATA  *RegsiteredData,
  IN EFI_KEY_DATA  *InputData
  );

/**
  Timer event handler: read a series of keycodes from I2C
  and put them into memory keycode buffer.
  it read as much keycodes to either fill
  the memory buffer or empty the keyboard buffer.
  It is registered as running under TPL_NOTIFY

  @param Event       The timer event
  @param Context     A I2C_KB_DEV pointer

**/
VOID
EFIAPI
I2CKeyboardTimerHandler (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  Sends the output Report Request 

  @param  I2CKeyboardDevice  The I2C_KB_DEV instance.
  @param  Led                Led data

  @retval EFI_SUCCESS        Output report sent successfully.
  @retval I2C_ERROR_INVALID_PARAMETER   I2C handle is not configured properly.
  @retval EFI_DEVICE_ERROR        Device did not respond.

**/
EFI_STATUS  
SetOutputReport ( 
  IN  I2C_KB_DEV    *I2cKeyboardDevice,
  IN  VOID          *Led
  );

/**
  Sets I2C keyboard LED state.

  @param  I2CKeyboardDevice  The I2C_KB_DEV instance.

  @retval EFI_SUCCESS        Keyboard LED is lighten successfully.
  @retval I2C_ERROR_INVALID_PARAMETER   I2C handle is not confiured properly.
  @retval EFI_DEVICE_ERROR        Device did not respond.

**/
EFI_STATUS 
SetKeyboardLED (  
  IN  I2C_KB_DEV    *I2CKeyboardDevice  
  );

#endif

