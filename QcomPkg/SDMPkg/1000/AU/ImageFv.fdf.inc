# @file ImageFv.fdf.inc
# FLASH layout file for ImageFv.
#
# Copyright (c) 2018,2019 Qualcomm Technologies, Inc. All rights reserved.
# Portions Copyright (c) 2009, Apple Inc. All rights reserved.
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#

[FV.ImageFv]
BlockSize          = 0x40
NumBlocks          = 0x0
FvAlignment        = 8
ERASE_POLARITY     = 1
MEMORY_MAPPED      = TRUE
STICKY_WRITE       = TRUE
LOCK_CAP           = TRUE
LOCK_STATUS        = TRUE
WRITE_DISABLED_CAP = TRUE
WRITE_ENABLED_CAP  = TRUE
WRITE_STATUS       = TRUE
WRITE_LOCK_CAP     = TRUE
WRITE_LOCK_STATUS  = TRUE
READ_DISABLED_CAP  = TRUE
READ_ENABLED_CAP   = TRUE
READ_STATUS        = TRUE
READ_LOCK_CAP      = TRUE
READ_LOCK_STATUS   = TRUE
FvNameGuid         = a8169396-d0f7-49cb-890a-25e1a9767406
  
  FILE FREEFORM = 3E5584ED-05D4-4267-9048-0D47F76F4248 {
    SECTION UI = "battery_symbol_Soc10.bmp"
    SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/battery_symbol_Soc10.bmp
  }

  FILE FREEFORM = 4753E815-DDD8-402d-BF69-9B8C4EB7573E {
      SECTION UI = "battery_symbol_NoBattery.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/battery_symbol_NoBattery.bmp
  }

  FILE FREEFORM = 03DED53E-BECD-428f-9F79-5ABA64C58445 {
      SECTION UI = "battery_symbol_Nocharger.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/battery_symbol_Nocharger.bmp
  }

  FILE FREEFORM = 8b86cd38-c772-4fcf-85aa-345b2b3c1ab4 {
      SECTION UI = "battery_symbol_LowBatteryCharging.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/battery_symbol_LowBatteryCharging.bmp
  }

  FILE FREEFORM = 3FD97907-93F1-4349-AF3C-3B68B0A5E626 {
      SECTION UI = "battery_symbol_LowBattery.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/battery_symbol_LowBattery.bmp
  }

  FILE FREEFORM = E2EA5A4F-98BD-4203-AB49-399505DDFB7D {
      SECTION UI = "tsens_thermal_symbol.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/tsens_thermal_symbol.bmp
  }

  FILE FREEFORM = C72FB1A9-5527-4E13-BAC4-F671D0330145 {
      SECTION UI = "tsens_thermal_err_symbol.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/tsens_thermal_err_symbol.bmp
  }

  FILE FREEFORM = B0F8D1BE-5353-4812-B1F6-07E7768204CC {
      SECTION UI = "battery_symbol_DebugBoot.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/battery_symbol_DebugBoot.bmp
  }

  FILE FREEFORM = F9E8F683-E065-4E09-B4F9-0230D7CECD08 {
      SECTION UI = "battery_symbol_DebugStay.bmp"
      SECTION RAW = QcomPkg/Drivers/QcomChargerDxe/battery_symbol_DebugStay.bmp
  }


[FV.IMAGEFV_COMPACT]
BlockSize          = 0x200
NumBlocks          = 0x10
FvAlignment        = 8
ERASE_POLARITY     = 1
MEMORY_MAPPED      = TRUE
STICKY_WRITE       = TRUE
LOCK_CAP           = TRUE
LOCK_STATUS        = TRUE
WRITE_DISABLED_CAP = TRUE
WRITE_ENABLED_CAP  = TRUE
WRITE_STATUS       = TRUE
WRITE_LOCK_CAP     = TRUE
WRITE_LOCK_STATUS  = TRUE
READ_DISABLED_CAP  = TRUE
READ_ENABLED_CAP   = TRUE
READ_STATUS        = TRUE
READ_LOCK_CAP      = TRUE
READ_LOCK_STATUS   = TRUE

  FILE FV_IMAGE = 072BE68A-6EC0-B209-9B83-276F017EBEAA { 
    SECTION GUIDED EE4E5898-3914-4259-9D6E-DC7BD79403CF PROCESSING_REQUIRED = TRUE {
      SECTION FV_IMAGE = IMAGEFV
    }
  }
