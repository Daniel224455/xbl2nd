#==============================================================================
# @file Loader.dsc
# SDM1000 Loader package.
#
#          Copyright (c) 2017-2018 QUALCOMM Technologies, Inc.
#               All Rights Reserved.
#     Qualcomm Technologies Inc. Proprietary and Confidential.
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
  QcomPkg/XBLLoader/XBLLoader.inf {
    <LibraryClasses>
      DevPrgLLib|QcomPkg/Library/NullLibs/DevPrgLLibNull/DevPrgLLibNull.inf
      DevPrgDLib|QcomPkg/Library/NullLibs/DevPrgDLibNull/DevPrgDLibNull.inf
      BootDdrTrainingLib|QcomPkg/XBLLoader/BootDdrTrainingLib.inf
      BootDebugLib|QcomPkg/XBLLoader/BootDebugLib.inf
      QusbLdrLib|QcomPkg/SDMPkg/1000/Library/QusbLdrLib/QusbLdrLib.inf
      VSenseTargetLib|QcomPkg/Library/NullLibs/VSenseTargetLibNull/VSenseTargetLibNull.inf
      SdccCommonLib|QcomPkg/Library/SdccCommonLib/SdccCommonReducedLib.inf
      ProcLib|QcomPkg/Library/ProcLib/ProcLib.inf
      ClockTargetLib|QcomPkg/SDMPkg/1000/Library/ClockTargetLib/ClockTargetLib.inf
      # SPMI Library
      SPMILib|QcomPkg/SDMPkg/1000/Library/SPMILib/SPMILib.inf

      # Only required for XBL loader
      #  CPRTargetLib|QcomPkg/SDMPkg/1000/Library/CPRTargetLib/Loader/CPRTargetLib.inf
      #CPRTargetLib|QcomPkg/Library/NullLibs/PowerLibNull/CprLibNull.inf
      #RailwayTargetLib|QcomPkg/SDMPkg/1000/Library/RailwayTargetLib/RailwayTargetLib.inf
      HashLib|QcomPkg/Library/NullLibs/CryptoDriversLibNull/CeLibNull.inf
      BootTempCheckLib|QcomPkg/Library/BootTempCheckLib/BootTempCheckLib.inf
      #HashLib|QcomPkg/SDMPkg/Library/HashLibLoader/HashLibHw.inf
      #This PCD values should match with the ones defined in boot_sbl_shared.h for sbl_build_type.
      <PcdsFixedAtBuild>
        gQcomTokenSpaceGuid.PcdBuildType|0x0
        gQcomTokenSpaceGuid.SDCCDmaEnabled|TRUE
        gQcomTokenSpaceGuid.SDCCSdhciEnable|TRUE
        gQcomTokenSpaceGuid.SDCCEmmcSpeedMode|0
  }

