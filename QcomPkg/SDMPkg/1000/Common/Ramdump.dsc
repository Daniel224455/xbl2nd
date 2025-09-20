#==============================================================================
# @file Loader.dsc
# SDM1000 Loader package.
#
# Copyright (c) 2017 - 2019, 2023 Qualcomm Technologies, Inc. All rights reserved.
# Confidential and Proprietary - Qualcomm Technologies, Inc.
#==============================================================================

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = SDM1000Pkg
  PLATFORM_GUID                  = AA99686E-4D75-3829-5220-F4874BA2F216
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

  OUTPUT_DIRECTORY               = Build/SDM1000_Loader

!include QcomPkg/SDMPkg/1000/Common/TargetCommon.dsc.inc

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
#
# SEC
#
  QcomPkg/SDMPkg/Library/XBLRamDumpLib/XBLRamDumpLib.inf {
    <LibraryClasses>
      QusbTargetLib|QcomPkg/SDMPkg/1000/Library/QusbTargetLib/QusbTargetLib.inf
      SdccCommonLib|QcomPkg/Library/SdccCommonLib/SdccCommonRamdumpLib.inf
      ClockTargetLib|QcomPkg/SDMPkg/1000/Library/ClockTargetLib/ClockTargetLib.inf
      ClockRamDumpLib|QcomPkg/SDMPkg/1000/Library/ClockTargetLib/ClockRamDumpLib.inf
      PmicLib|QcomPkg/SDMPkg/1000/Library/PmicLib/PmicLib.inf

      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf

      DisplayLib|QcomPkg/Library/DisplayLib/DisplayLib.inf

      HALDSILib|QcomPkg/Library/HALDSILib/HALDSILib.inf
      HALEDPLib|QcomPkg/Library/HALDPLib/HALEDPLib.inf
      ExternalDisplayLibStub|QcomPkg/Library/ExternalDisplayLib/ExternalDisplayLibStub.inf
      HALMDSSLib|QcomPkg/Library/HALMDSSLib/HALMDSS5xx.inf
      MDPLibBoot|QcomPkg/Library/MDPLib/MDPLibBoot.inf
      MDPPlatformLibBoot|QcomPkg/SDMPkg/1000/Library/MDPPlatformLibBoot/MDPPlatformLibBoot.inf

      #CeLibNull|QcomPkg/Library/NullLibs/CryptoDriversLibNull/CeLibNull.inf
      PCIeHostLib|QcomPkg/SDMPkg/1000/Library/PCIeHostTargetLib/pcie_host.inf
      NvmeLoaderLib|QcomPkg/Library/NvmeLoaderLib/NvmeLoaderLib.inf
      NvmeLoaderTargetLib|QcomPkg/SDMPkg/1000/Library/NvmeTargetLib/NvmeLoaderTargetLib.inf
      RNGLib|QcomPkg/SDMPkg/Library/RNGLoaderLib/RNGLibHw.inf
      HashLib|QcomPkg/SDMPkg/Library/HashLibLoader/HashLibHw.inf
      CryptoLib|QcomPkg/SDMPkg/Library/CryptoLib/CryptoLib.inf
      <PcdsFixedAtBuild>
        gQcomTokenSpaceGuid.SDCCDmaEnabled|TRUE
        gQcomTokenSpaceGuid.SDCCSdhciEnable|TRUE
        gQcomTokenSpaceGuid.SDCCEmmcSpeedMode|0
  }

