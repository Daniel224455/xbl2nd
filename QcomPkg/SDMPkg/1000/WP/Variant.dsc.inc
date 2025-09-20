#==============================================================================
# @file Common_855.dscinc
# SDM855 Loader package.
#
# Copyright (c) 2017 Qualcomm Technologies, Inc. All rights reserved.
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
  PmicLib|QcomPkg/SDMPkg/1000/Library/PmicLib/PmicLib.inf
  PmicLoaderLib|QcomPkg/SDMPkg/1000/Library/PmicLib/loader/WP/PmicLoaderLib.inf
  DRETargetLib|QcomPkg/SDMPkg/Library/DRETargetLibNull/DRETargetLibNull.inf
  DDRTargetLib|QcomPkg/SDMPkg/1000/Library/DDRTargetLib/DDRTargetLib.inf
  CPRTargetLib|QcomPkg/SDMPkg/1000/Library/CPRTargetLib/Loader/CPRTargetLib.inf
  RailwayTargetLib|QcomPkg/SDMPkg/1000/Library/RailwayTargetLib/RailwayTargetLib.inf
  DSFTargetLib|QcomPkg/SDMPkg/1000/Library/DSFTargetLib/DSFTargetLib.inf
  BootCdtLib|QcomPkg/SDMPkg/Library/XBLLoaderLib/BootCdtLib.inf 
  StorageProfileLib|QcomPkg/Library/StorageCommonLib/StorageProfileStub.inf   
  TLMMTargetLib|QcomPkg/SDMPkg/1000/Library/TLMMTargetLib/TLMMTargetLib_AU.inf