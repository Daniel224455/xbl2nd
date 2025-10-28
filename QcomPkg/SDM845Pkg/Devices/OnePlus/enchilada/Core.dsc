#==============================================================================
# @file Core.dsc
# SDM845 Core package.
#
# Copyright (c) 2015 - 2018 Qualcomm Technologies, Inc. All rights reserved.
# Portions Copyright (c) 2009 - 2010, Apple Inc. All rights reserved.
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#==============================================================================
# XBL2nd: Please mark changed stuff with # DEVICE_CUST, thank you
################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = SDM845Pkg
  PLATFORM_GUID                  = 23766BF3-7B8F-41BF-99A7-D06F68212A36
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  SUPPORTED_ARCHITECTURES        = ARM|AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = QcomPkg/SDM845Pkg/Devices/OnePlus/enchilada/Core.fdf

[Defines.ARM]
  OUTPUT_DIRECTORY               = Build/SDM845LA_Core_32

[Defines.AARCH64]
  OUTPUT_DIRECTORY               = Build/SDM845LA_Core

!include QcomPkg/SDM845Pkg/LA/NapaliShared.dsc.inc

[PcdsFixedAtBuild.common]
#
# SMBIOS related
#
  # OEM's can set their product info here
  gQcomTokenSpaceGuid.PcdSystemMfrStr|"Qualcomm"
  gQcomTokenSpaceGuid.PcdSystemProductNameStr|""
  gQcomTokenSpaceGuid.PcdSystemProductFamilyStr|""

#
# Display PCDs
#
  gQcomTokenSpaceGuid.PcdPanelType|0
  gQcomTokenSpaceGuid.PcdHPDPollCount|200
  gQcomTokenSpaceGuid.PcdBacklightLevel|50
  gQcomTokenSpaceGuid.PcdDisplayForceSwRenderer|FALSE
  # gFBResLimit Table 0: 4k, 1: UHD, 2: WQHD, 3: FHD, 4: HD
  gQcomTokenSpaceGuid.PcdFrameBufMaxRes|0
  # External display type:  0: No external display, 1: DisplayPort, 2: HDMI
  gQcomTokenSpaceGuid.PcdExtDisplayType|0
  # External display mode Index : 0: Preferred mode, 1: Largest mode
  gQcomTokenSpaceGuid.PcdSetExtMode|0
  gQcomTokenSpaceGuid.PcdReservedMemorySize|0
  gQcomTokenSpaceGuid.PcdReservedMemoryAlignment|64
  gQcomTokenSpaceGuid.PcdBacklightLevel|50
  gQcomTokenSpaceGuid.PcdPresentationMode|0

#
# Target Specific PCDs
#

  #########################################################################
  #                               Memory Map
  #########################################################################

  gQcomTokenSpaceGuid.PcdMaxMemory|0x0

  #################################
  ## Presilicon definitions.
  #################################
  gQcomTokenSpaceGuid.PcdMemoryBase|0x80000000    # Starting address
  gQcomTokenSpaceGuid.PcdMemorySize|0x60000000    # 1.5 GB

  #################################
  ## HLOS Base Offset and size.
  #################################
  gQcomTokenSpaceGuid.PcdHLOSMemoryBaseOffset|0x98200000 # remaining memory after this is HLOS

  #################################
  ## UEFI Memory Base Offset and Size.
  #################################
  gQcomTokenSpaceGuid.PcdUefiMemPoolBaseOffset|0x94E00000
  gQcomTokenSpaceGuid.PcdUefiMemPoolSize|0x03300000     # 51 MB

  # The HOB list will start here.
  # NOTE: Hob.c is still referencing PcdPrePiHobBase
  gQcomTokenSpaceGuid.PcdPrePiHobBase|0x94E00000

  #################################
  ## FD Base offset (refer to .fdf for FD size)
  #################################
  gQcomTokenSpaceGuid.PcdEmbeddedFdBaseAddress|0x9FC00000

  #################################
  ## Shared Memory Base offset and size.
  ## TODO: PcdSmemBaseAddress is currently used by smem library
  #################################
  gQcomTokenSpaceGuid.PcdSmemBaseAddress|0x86000000  #64-bit smem addr
  gQcomTokenSpaceGuid.PcdSmemSize|0x00200000     # 2MB
  ## Generic SMEM information that SMEM lib might need
  ## LSB represents chip family, other bytes are unused for now.
  gQcomTokenSpaceGuid.PcdSmemInformation|0x00000005  # 5 = Nazgul

  # Maximum number of memory regions defined in the above memory map
  gQcomTokenSpaceGuid.PcdMaxMemRegions|64

  #################################
  ## IMem Base Address and size.
  #################################

  gQcomTokenSpaceGuid.PcdIMemCookiesBase|0x146BF000
  gQcomTokenSpaceGuid.PcdIMemCookiesSize|0x00001000

  # Cookie (memory capture mode) Address for crash dump support
  gQcomTokenSpaceGuid.PcdHLOSCrashCookieAddr|0x146BF018

  # USB
  gQcomTokenSpaceGuid.InitUsbControllerOnBoot|FALSE
  gQcomTokenSpaceGuid.HostModeSupported|FALSE
  gQcomTokenSpaceGuid.USBSmmuStage1Supported|TRUE

[PcdsPatchableInModule]
  ## This PCD defines the Console output column and the default value is 25 according to UEFI spec
  #gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|53

  ## This PCD defines the Console output row and the default value is 80 according to UEFI spec
  #gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|75

  # This PCD should be set to 0 then video resolution could be at highest resolution.
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|2240 # DEVICE_CUST
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1080 # DEVICE_CUST

################################################################################
#
# UserExtensions.Buildit.BuildInfo - Extra info for buildit.py
#
################################################################################
[UserExtensions.Buildit.BuildInfo]
  COMPILER_ARCH = AARCH64
  OUTPUT_NAME   = uefi.elf
  SIGN_FD       = uefi
  MERGE         = xbl.elf
  SIGN          = sbl1
  COMPILER_WIN  = CLANG39WIN
  COMPILER_LINUX = CLANG39LINUX
