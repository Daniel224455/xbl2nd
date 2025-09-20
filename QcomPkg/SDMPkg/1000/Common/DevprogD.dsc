#==============================================================================
# @file DevprogD.dsc
# SDM1000 Device Programmer
#
# Copyright (c) 2017 Qualcomm Technologies, Inc. All rights reserved.
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
  QcomPkg/XBLLoader/DevPrgD.inf {
     <LibraryClasses>
      XBLLoaderLib|QcomPkg/SDMPkg/Library/XBLLoaderLib/XBLLoaderDevProgLib.inf
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
      SmemLib|QcomPkg/Library/NullLibs/SmemLibNull/SmemLibNull.inf
      DevPrgLLib|QcomPkg/Library/NullLibs/DevPrgLLibNull/DevPrgLLibNull.inf
      DevPrgDLib|QcomPkg/SDMPkg/Library/DevPrgDLib/DevPrgDLib.inf
      BootDdrTrainingLib|QcomPkg/XBLLoader/BootDdrTrainingLibNull.inf
      BootDebugLib|QcomPkg/XBLLoader/BootDebugLibNull.inf
      VSenseTargetLib|QcomPkg/Library/NullLibs/VSenseTargetLibNull/VSenseTargetLibNull.inf
      ColdplugLib|QcomPkg/Library/NullLibs/ColdplugLibNull/ColdplugLibNull.inf
      LoaderAptLib|QcomPkg/Library/NullLibs/LoaderAptLibNull/LoaderAptLibNull.inf
      QusbTargetLib|QcomPkg/SDMPkg/1000/Library/QusbTargetLib/QusbTargetLib.inf
      QusbLdrLib|QcomPkg/Library/NullLibs/QusbLdrLibNull/QusbLdrLibNull.inf
      CPRTargetLib|QcomPkg/Library/NullLibs/PowerLibNull/CprLibNull.inf
      XcfgLoaderElf|QcomPkg/Library/XBLConfig/XcfgRamElfLib.inf
      ProcLib|QcomPkg/Library/ProcLib/ProcLib.inf
      ClockTargetLib|QcomPkg/SDMPkg/1000/Library/ClockTargetLib/ClockTargetLib.inf
      HashLib|QcomPkg/SDMPkg/Library/HashLibLoader/HashLibHw.inf
      PCIeHostLib|QcomPkg/SDMPkg/1000/Library/PCIeHostTargetLib/pcie_host.inf
      NvmeLoaderLib|QcomPkg/Library/NvmeLoaderLib/NvmeLoaderLib.inf
      NvmeLoaderTargetLib|QcomPkg/SDMPkg/1000/Library/NvmeTargetLib/NvmeLoaderTargetLib.inf
	  DRETargetLib|QcomPkg/SDMPkg/Library/DRETargetLibNull/DRETargetLibNull.inf
      BootTempCheckLib|QcomPkg/Library//NullLibs/BootTempCheckLibNull/BootTempCheckLibNull.inf
      <PcdsFixedAtBuild>
        gQcomTokenSpaceGuid.PcdBuildType|0x2
   }

[BuildOptions.AARCH64]
  GCC:*_*_AARCH64_ARCHCC_FLAGS  = -DFEATURE_DEVICEPROGRAMMER_IMAGE
