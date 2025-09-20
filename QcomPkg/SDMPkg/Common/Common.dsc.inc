#==============================================================================
# @file SDM1000Pkg_Loader.dsc
# SDM1000 Loader package.
#
# Copyright (c) 2017-2019 Qualcomm Technologies, Inc. All rights reserved.
# Portions Copyright (c) 2009 - 2010, Apple Inc. All rights reserved.
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#==============================================================================
[LibraryClasses.common]
  # EDK2 Libraries
  NULL|ArmPkg/Library/CompilerIntrinsicsLib/CompilerIntrinsicsLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|ArmPkg/Library/BaseMemoryLibStm/BaseMemoryLibStm.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

  # Qcom Generic Libraries
  AdcLoaderLib|QcomPkg/Library/AdcLib/build/AdcLoaderLib.inf
  #Bam Lib
  BamLoaderLib|QcomPkg/Library/BamLib/BamLoaderLib.inf
  BamTargetLib|QcomPkg/SDMPkg/Library/BamTargetLib/BamTargetLib.inf
  BootDisplayLib|QcomPkg/Library/BootDisplayLib/BootDisplayLib.inf
  BusyWaitLib|QcomPkg/Library/BusyWaitLib/BusyWaitLib.inf
  ChipInfoLoaderLib|QcomPkg/Library/ChipInfoLib/ChipInfoLoader.inf
  CoreUtilsLib|QcomPkg/Library/CoreUtilsLib/CoreUtilsLib.inf
  DALCommonLoaderLib|QcomPkg/Library/DALCommonLib/DALCommonLoaderLib.inf
  DALFwkLib|QcomPkg/Library/DALFwkLib/DALFwkLib.inf
  FontLib|QcomPkg/Library/FontLib/FontLib.inf
  HFATLib|QcomPkg/Library/HFATLib/HFATLib.inf
  ColdplugEfsLib|QcomPkg/Library/ColdplugLib/ColdplugEfsLib.inf
  ColdplugLib|QcomPkg/Library/ColdplugLib/ColdplugSpiNorLib.inf

  ImageVersionLib|QcomPkg/Library/ImageVersionLib/ImageVersionLib.inf
  LoaderCacheLib|QcomPkg/XBLLoader/LoaderCacheLib.inf
  XBLConfigLib|QcomPkg/Library/XBLConfig/XBLConfigLib.inf
  XcfgLoaderElf|QcomPkg/Library/XBLConfig/XcfgLoaderElfLib.inf
  MuslLib|QcomPkg/Library/MuslLib/MuslLib.inf
  NULL|QcomPkg/Library/MuslLib/MuslLib.inf
  NULL|QcomPkg/Library/StackCanaryBaseLib/StackCanaryBaseLibLoader.inf
  PlatformInfoLoaderLib|QcomPkg/Library/PlatformInfoLib/PlatformInfoLoader.inf
  QusbLib|QcomPkg/Library/QusbLib/QusbLib.inf
  ROMFSLib|QcomPkg/Library/EfsLib/ROMFSLib.inf
  RailwayLib|QcomPkg/Library/RailwayLib/RailwayLib.inf
  SPILib|QcomPkg/Library/SPILib/SPILib.inf
  SPIFWImageLib|QcomPkg/SDMPkg/1000/Settings/SPI/SPIFWImageLib.inf
  SPMIApiLib|QcomPkg/Library/SPMIApiLib/SPMIApiLib.inf
  SdccCommonLib|QcomPkg/Library/SdccCommonLib/SdccCommonLib.inf
  SdccHalLib|QcomPkg/Library/SdccCommonLib/SdccHalLib/SdccHalLib.inf
  StackCanaryBaseLib|QcomPkg/Library/StackCanaryBaseLib/StackCanaryBaseLibLoader.inf
  ClockLoaderLib|QcomPkg/Library/ClockLoaderLib/ClockLoaderLib.inf
  UartLib|QcomPkg/Library/UartQupv3Lib/UartLoaderLib.inf
  ServicesUtilsLib|QcomPkg/Library/ServicesLib/ServicesUtilsLib.inf
  SmemLib|QcomPkg/Library/SmemLib/build/SmemLibXBLLoader.inf
  SpiNorCommonLib|QcomPkg/Library/SpiNorCommonLib/SpiNorCommonLib.inf
  TimetickLib|QcomPkg/Library/TimetickLibB/TimetickLib.inf
  UfsJtagLib|QcomPkg/Library/UfsCommonLib/UfsJtagLib.inf
  UfsLoaderLib|QcomPkg/Library/UfsCommonLib/UfsLoaderLib.inf
  PrngLib|QcomPkg/SDMPkg/Library/RNGLoaderLib/RNGLibHw.inf

  # TSENS Libraries
  TsensLoaderLib|QcomPkg/Library/TsensLib/TsensLoaderLib.inf


  # Security Libs (TBD: To remove, no longer needed)
  UIELib|QcomPkg/Library/NullLibs/UIELibNull/UIELibNull.inf
  AptLib|QcomPkg/Library/NullLibs/AptLibNull/AptLibNull.inf
  CryptoRDILib|QcomPkg/SDMPkg/Library/CryptoInterfaceLib/CryptoRDILib.inf
  CryptoLoaderILib|QcomPkg/SDMPkg/Library/CryptoInterfaceLib/CryptoLoaderILib.inf

  # Target Libraries
  XBLLoaderLib|QcomPkg/SDMPkg/Library/XBLLoaderLib/XBLLoaderLib.inf
  XBLCommonLib|QcomPkg/SDMPkg/Library/XBLLoaderLib/XBLCommonLib.inf
  DDRTargetLib|QcomPkg/Library/NullLibs/DDRTargetLibNull/DDRTargetLibNull.inf
  #DSFTargetLib|QcomPkg/SDMPkg/Library/DSFTargetLib/DSFTargetLib.inf
  SpinorCoreTargetLib|QcomPkg/SDMPkg/Library/SpinorTargetLib/SpinorCoreTargetLib.inf

  TlmmLib|QcomPkg/Library/TlmmLib/TlmmLib.inf
  SmemTargetLib|QcomPkg/SDMPkg/Library/SmemTargetLib/SmemTargetLib.inf
  DisplayLib|QcomPkg/Library/NullLibs/DisplayLibNull/DisplayLibNull.inf
  UsbWrapperLayerLib|QcomPkg/SDMPkg/Library/UsbWrapperLayerLib/UsbWrapperLayerLib.inf

  LockLib|QcomPkg/Library/LockLib/LockLibNull.inf

  # SDCC
  SdccClockLib|QcomPkg/SDMPkg/Library/SdccClockLib/SdccLoaderClockLib.inf

  #STI
  LoaderAptLib|QcomPkg/Library/LoaderAptLib/LoaderAptLib.inf
  LoaderAptLibNull|QcomPkg/Library/NullLibs/LoaderAptLibNull/LoaderAptLibNull.inf

  #NULL Target Reset
  TargetResetLib|QcomPkg/Library/NullLibs/TargetResetLibNull/TargetResetLib.inf
  QcomBaseLib|QcomPkg/Library/NullLibs/QcomBaseLibNull/QcomBaseLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag.common]

[PcdsFixedAtBuild.common]

#
# Target Specific PCDs
#
  #################################
  ## PcdSmemBaseAddress is currently used by smem library
  #################################
  gQcomTokenSpaceGuid.PcdSmemBaseAddress|0x86000000  #64-bit smem addr
  gQcomTokenSpaceGuid.PcdSmemSize|0x00200000         # 2MB
  ## Generic SMEM information that SMEM lib might need
  ## LSB represents chip family, other bytes are unused for now.
  gQcomTokenSpaceGuid.PcdSmemInformation|0x00000005  # 5 = Nazgul

  # Timers
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x9BC0000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x65C0000
  gQcomTokenSpaceGuid.PcdQTimerAC_BasePhys|0x17C20000
  gQcomTokenSpaceGuid.PcdQTimerV1BasePhys|0x17C21000
  gQcomTokenSpaceGuid.PcdQTimerV2BasePhys|0x17C22000
  gQcomTokenSpaceGuid.PcdQTimerFrame|0
  gQcomTokenSpaceGuid.PcdQTimerEnabled|1


  # SDCC
  gQcomTokenSpaceGuid.SDCCExtSlotNumber|1
  gQcomTokenSpaceGuid.SDCCEmmcSlotNumber|0
  gQcomTokenSpaceGuid.SDCCRpmbKey|{0x20, 0xAA, 0x9C, 0xF4, 0x99, 0x4D, 0xD4, 0xFE, 0xA5, 0x85, 0xBE, 0x96, 0x6, 0x1, 0xD1, 0xA9, 0xC3, 0x3, 0x4F, 0x91, 0x62, 0x7C, 0x64, 0x53, 0x38, 0xC1, 0x1F, 0xF2, 0x76, 0x4D, 0x2E, 0xC4}
  gQcomTokenSpaceGuid.SDCCEventBasedSDDetectEnabled|FALSE
  gQcomTokenSpaceGuid.SDCCDmaEnabled|TRUE
  gQcomTokenSpaceGuid.SDCCEmmcHwResetEnabled|TRUE
  gQcomTokenSpaceGuid.SDCCMaxSlotNumber|2
  gQcomTokenSpaceGuid.SDCCSdhciEnable|TRUE
  ## SDCC eMMC speed mode - DDR50: 0, HS200: 1, HS400: 2 SDR: 3 ##
  gQcomTokenSpaceGuid.SDCCEmmcSpeedMode|1

