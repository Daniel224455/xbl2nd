#==============================================================================
# @file JtagProgrammer.dsc
# SDM1000 JtagProgrammer
#
# Copyright (c) 2017-2018 Qualcomm Technologies, Inc. All rights reserved.
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
  QcomPkg/Library/JtagProgrammerLib/JtagProgrammer.inf {
      <LibraryClasses>
        ClockTargetLib|QcomPkg/SDMPkg/1000/Library/ClockTargetLib/ClockTargetLib.inf
        TLMMTargetLib|QcomPkg/SDMPkg/1000/Library/TLMMTargetLib/TLMMTargetLib.inf
        SpinorTargetLib|QcomPkg/SDMPkg/1000/Library/SpinorTargetLib/jtag/SpinorTargetLib.inf
        ProcLib|QcomPkg/Library/ProcLib/ProcLib.inf
		StorageProfileLib|QcomPkg/Library/StorageCommonLib/StorageProfileStub.inf

      <PcdsFixedAtBuild>
        gQcomTokenSpaceGuid.PcdBuildType|0x4
        gQcomTokenSpaceGuid.SDCCDmaEnabled|FALSE
        gQcomTokenSpaceGuid.SDCCSdhciEnable|TRUE
        gQcomTokenSpaceGuid.SDCCEmmcSpeedMode|0
   }

