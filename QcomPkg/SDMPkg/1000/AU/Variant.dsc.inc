#==============================================================================
# @file Common_855.dscinc
# SDM855 Loader package.
#
# Copyright (c) 2017, 2020 Qualcomm Technologies, Inc. All rights reserved.
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
# Need to make AU specific
  PmicLib|QcomPkg/SDMPkg/1000/Library/PmicLib/PmicAuLib.inf
  PmicLoaderLib|QcomPkg/SDMPkg/1000/Library/PmicLib/loader/AU/PmicLoaderLib.inf
  CPRTargetLib|QcomPkg/SDMPkg/1000/Library/CPRTargetLib/Loader/CPRTargetLibAu.inf
  RailwayTargetLib|QcomPkg/SDMPkg/1000/Library/RailwayTargetLib/RailwayTargetAuLib.inf
  DDRTargetLib|QcomPkg/SDMPkg/1000/Library/DDRTargetLib_AU/DDRTargetLib.inf
  DSFTargetLib|QcomPkg/SDMPkg/1000/Library/DSFTargetLib_AU/DSFTargetLib.inf
  DRETargetLib|QcomPkg/SDMPkg/1000/Library/DRETargetLib/DRETargetLib.inf
  BootCdtLib|QcomPkg/SDMPkg/Library/XBLLoaderLib/BootCdtAULib.inf    
  StorageProfileLib|QcomPkg/Library/StorageCommonLib/StorageProfile.inf
  StorageBootCommonLib|QcomPkg/Library/StorageCommonLib/StorageLoaderCommon.inf
  TLMMTargetLib|QcomPkg/SDMPkg/1000/Library/TLMMTargetLib/TLMMTargetLib_AU.inf

[BuildOptions.AARCH64]
  GCC:*_*_*_CC_FLAGS = -DBOOT_CRASHDUMP_FEATURE -DENABLE_AUTO_PLAT
