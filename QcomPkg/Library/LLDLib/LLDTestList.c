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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <stdio.h>
#include <stdlib.h>
#include "LLD.h"

/*===============================================================================
  Button Test
================================================================================*/

static const TF_ParamDescr ButtonParam[] = {
  //put descriptions of all input parameters here
};

static const TF_HelpDescr ButtonHelp = {
    "Used to test the functionality of main physical hardware buttons (VUp, VDown, Pwr, Camera)",
    sizeof(ButtonParam) / sizeof(TF_ParamDescr),
    ButtonParam
};

/*===============================================================================
  Camera Connectivity Test
================================================================================*/

static const TF_ParamDescr CameraConnectivityParam[] = {
};

static const TF_HelpDescr CameraConnectivityHelp = {
    "Checks Camera Sensor ID to test Camera Connectivity",
    sizeof(CameraConnectivityParam) / sizeof(TF_ParamDescr),
    CameraConnectivityParam
};

/*===============================================================================
  Charger Test
================================================================================*/

static const TF_ParamDescr ChargerParam[] = {
};

static const TF_HelpDescr ChargerHelp = {
    "Prints the charging status",
    sizeof(ChargerParam) / sizeof(TF_ParamDescr),
    ChargerParam
};

/*===============================================================================
  ChipInfo Test
================================================================================*/

extern EFI_STATUS EFIAPI ChipInfoAppMain(IN EFI_HANDLE ImageHandle,
                                         IN EFI_SYSTEM_TABLE *SystemTable);

uint32 ChipInfoTestMain(uint32 dwParam, char *apszParam[]) {
    return ChipInfoAppMain(NULL, NULL);
}

static const TF_ParamDescr ChipInfoAppParam[] = {
};

static const TF_HelpDescr ChipInfoAppHelp = {
    "Prints the ASIC Attributes to the screen",
    sizeof(ChipInfoAppParam) / sizeof(TF_ParamDescr),
    ChipInfoAppParam
};

/*===============================================================================
  Display Test
================================================================================*/

/* List of display test cases 
 TestCaseEntry TestCaseList[] = 
 {
   {"Blt Performance",    &DisplayBltPerformanceTest},
   {"Blt Operation",      &DisplayBltOperationTest},
   {"Power Control",      &DisplayPanelPowerControlTest},
   {"Power State",        &DisplayPanelPowerStateTest},
   {"Power Status",       &DisplayPanelPowerStatusTest},
   {"Brightness Control", &DisplayBacklightBrightnessLevelControlTest},  
   {"Brightness Status",  &DisplayBacklightBrightnessLevelStatusTest},  
   {"Power Event",        &DisplayPowerEventTest}
 };*/

static const TF_ParamDescr DisplayParam[] = {
  {TF_PARAM_STRING, "1", "execute blt performance test"},
  {TF_PARAM_STRING, "2", "execute blt operation test"},
  {TF_PARAM_STRING, "3", "execute display panel power control test"},
  {TF_PARAM_STRING, "4", "execute display panel power state test"},
  {TF_PARAM_STRING, "5", "execute display panel power status test"},
  {TF_PARAM_STRING, "6", "execute backlight brightness level control test"},
  {TF_PARAM_STRING, "7", "execute backlight brightness level status test"},
  {TF_PARAM_STRING, "8", "execute power event test"}
};

static const TF_HelpDescr DisplayHelp = {
    "Prints a bitmap to the screen to test the display",
    sizeof(DisplayParam) / sizeof(TF_ParamDescr),
    DisplayParam
};

/*===============================================================================
  Memory Validation Test
================================================================================*/

static const TF_ParamDescr MemoryValidationParam[] = {
};

static const TF_HelpDescr MemoryValidationHelp = {
    "Used to validate memory",
    sizeof(MemoryValidationParam) / sizeof(TF_ParamDescr),
	  MemoryValidationParam
};

/*===============================================================================
  PMIC Test
================================================================================*/

extern EFI_STATUS PmicTestMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

UINT32 PMICsTestMain(uint32 dwParam, char *apszParam[]){
    return PmicTestMain(NULL, NULL);
}

static const TF_ParamDescr PMICParam[] = {
};

static const TF_HelpDescr PMICHelp = {
    "Tests the PMIC core to make its running properly",
    sizeof(PMICParam) / sizeof(TF_ParamDescr),
    PMICParam
};

/*===============================================================================
  Thermal Test
================================================================================*/

extern EFI_STATUS EFIAPI TsensTestMain(IN EFI_HANDLE ImageHandle,
                                IN EFI_SYSTEM_TABLE *SystemTable);


UINT32 ThermalSensorsTestMain(uint32 dwParam, char *apszParam[]){
    return TsensTestMain(NULL, NULL);
}

static const TF_ParamDescr ThermalParam[] = {
};

static const TF_HelpDescr ThermalHelp = {
    "Tests the temperature of different hardware components to test thermometer functionality",
    sizeof(ThermalParam) / sizeof(TF_ParamDescr),
    ThermalParam
};

/*===============================================================================
  USBfnLoopback Test
================================================================================*/

extern EFI_STATUS EFIAPI UsbfnLoopbackMain(IN EFI_HANDLE ImageHandle,
                                           IN EFI_SYSTEM_TABLE *SystemTable);

uint32 USBTestMain(uint32 dwParam, char *apszParam[]) {
	return UsbfnLoopbackMain(gImageHandle, NULL);
}

static const TF_ParamDescr USBfnLoopbackParam[] = {
};

static const TF_HelpDescr USBfnLoopbackHelp = {
    "Prints the USBfnTest Info",
    sizeof(USBfnLoopbackParam) / sizeof(TF_ParamDescr),
    USBfnLoopbackParam
};

/*===============================================================================
  UFS Test
================================================================================*/

extern EFI_STATUS EFIAPI SdccTestAppMain(IN EFI_HANDLE ImageHandle,
                                         IN EFI_SYSTEM_TABLE *SystemTable);

UINT32 UFSTestMain(uint32 dwParam, char *apszParam[]){
    return SdccTestAppMain(NULL, NULL);
} 

static const TF_ParamDescr UFSParam[] = {
};

static const TF_HelpDescr UFSHelp = {
    "Tests the UFS connectivity.",
    sizeof(UFSParam) / sizeof(TF_ParamDescr),
    UFSParam
};

/*===============================================================================
  Array of Function Descriptors for all tests 
================================================================================*/

const TF_TestFunction_Descr LLD_test_list[] = {
  {"Button", ButtonTestMain, &ButtonHelp, NULL, 0},
  {"Camera", CameraConnectivityTestMain, &CameraConnectivityHelp, NULL, 0},
  {"Charger", ChargerTestMain, &ChargerHelp, NULL, 0},
  {"ChipInfo", ChipInfoTestMain, &ChipInfoAppHelp, NULL, 0},
  {"Display", DisplayTestMain, &DisplayHelp, NULL, 0},
  {"Memory", MemoryValidationTestMain, &MemoryValidationHelp, NULL, 0},
  {"PMIC", PMICsTestMain, &PMICHelp, NULL, 0},
  {"Thermal", ThermalSensorsTestMain, &ThermalHelp, NULL, 0},
  {"USB", USBTestMain, &USBfnLoopbackHelp, NULL, 0},
  {"UFS", UFSTestMain, &UFSHelp, NULL, 0},
  {NULL}
};

