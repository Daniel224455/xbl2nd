/*===============================================================================
  Copyright (c) 2017 Qualcomm Technologies, Inc.  All Rights Reserved.
  
  FILE: 	LLDTestList.c
  DESCRIPTION:	LLD Test Setup
  
  REVISION HISTORY
  when       who     what, where, why
  --------   ----    ----------------------------------------------------------
  06/22/17   ianr    Created
  02/20/18   akeshan Put APIs for all tests directly into this file
================================================================================*/

#include <DDITF.h>
#include <string.h>
#include <Library/SocPlatTestUtilLib.h>
#include <Library/UefiLib.h>
#include <stdio.h>
#include <stdlib.h>

/*===============================================================================
  Button Test APIs
================================================================================*/

/**
  This tests all the hardware buttons and returns 0 when all the buttons are pressed

  @param[in] dwParam        Number of keys OEM wants to test.
  @param[in] apszParam      Array of scancodes for keys OEM wants to test.
                            Max number of scancodes that can be sent is 32.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.
  The test times out after 30 secs if all the expected buttons are not pressed and returns FAILURE

  Eg: if you want to test POWER button with scancode 0x102, call function with (1, ["102"])
**/
UINT32 ButtonTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  Camera Connectivity Test APIs
================================================================================*/

/**
  This validates that camera is present and initializes it.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS Init_CamRearSensor();

/**
  This verifies that we can read information from the camera.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS ReadCameraSensorRegister();

/**
  This de-initializes the power rails, GPIO lines etc for camera.
**/
void DeInit_CamRearSensor();

/**
  This tests camera functionality by validating that we can connect to it and read info from it.

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs for the test; not currently being used.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Call function with (0, NULL) right now.

**/
UINT32 CameraConnectivityTestMain (uint32 dwParam, char *apszParam[]);

/*===============================================================================
  Charger Test APIs
================================================================================*/

/**
  This validates that Charger is present.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS ChargerPresenceTest();

/**
  This validates that Battery is present.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS BatteryPresenceTest();

/**
  This validates that Charger and Battery are present and then tests if the device is charging.

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs 

  apszParam[0] = chargingTime = time in microseconds for battery charging
  apszParam[1] = chargingThreshold = min voltage increase in mV to declare that battery is charging
  apszParam[2] = batteryCapacity = max voltage of the battery 
  
  NOTE: if current voltage read from battery is greater than or equal to the batteryCapacity,
  then we cannot charge it further and will automatically fail the test 
  
  Default if no input Params are passed in: 30000 ms = 30 sec, 5 mV, 3.8 V

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
UINT32 ChargerTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  ChipInfo Test APIs
================================================================================*/

/**
  This lists out Chip Info.

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs for the test; not currently being used.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Call function with (0, NULL) right now.

**/
UINT32 ChipInfoTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  Display Test APIs
================================================================================*/

/**
  This runs through a variety of tests for testing the display.

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of display tests OEM wants to run.

  Tests that can be run at present are:
   1 = Blt Performance
   2 = Blt Operation
   3 = Power Control
   4 = Power State
   5 = Power Status
   6 = Brightness Control  
   7 = Brightness Status  
   8 = Power Event

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Eg: if you want to run test 1 and 3, call function with (2, ["1", "3"])

**/
UINT32 DisplayTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  Memory Validation Test APIs
================================================================================*/

/**
  This tests the entire DDR and makes sure address and data lines are working fine.

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs for the test; not currently being used.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Call function with (0, NULL) right now.

**/
UINT32 MemoryValidationTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  PMIC Test APIs
================================================================================*/

/**
  This tests presence and functionality of PMIC GPIOs

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs for the test; not currently being used.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Call function with (0, NULL) right now.

**/
UINT32 PMICsTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  Thermal Test APIs
================================================================================*/

/**
  This tests whether all the thermal sensors are working within the intended range of 20-70 C

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs for the test; not currently being used.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Call function with (0, NULL) right now.

**/
UINT32 ThermalSensorsTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  USBfnLoopback Test APIs
================================================================================*/

/**
  This tests USB functionality

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs for the test; not currently being used.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Call function with (0, NULL) right now.

**/
uint32 USBTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  UFS Test APIs
================================================================================*/

/**
  This tests UFS functionality

  @param[in] dwParam        Number of arguments passed into the function.
  @param[in] apszParam      Array of inputs for the test; not currently being used.

  @retval EFI_SUCCESS/0     The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

  Call function with (0, NULL) right now.

**/
UINT32 UFSTestMain(uint32 dwParam, char *apszParam[]);

/*===============================================================================
  Array of Function Descriptors for all tests 
================================================================================*/

/** 
  Can be called from test app. 
  Calls directly into each of the Main Entry points for the test listed above.
**/
const TF_TestFunction_Descr LLD_test_list[];

