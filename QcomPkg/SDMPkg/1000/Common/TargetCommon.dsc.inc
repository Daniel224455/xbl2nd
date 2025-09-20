#==============================================================================
# @file Common_1000.dsc.inc
# SDM1000 Loader package.
#
# Copyright (c) 2017 - 2018 Qualcomm Technologies, Inc. All rights reserved.
# Portions Copyright (c) 2009 - 2010, Apple Inc. All rights reserved.
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#==============================================================================


!include QcomPkg/SDMPkg/Common/Common.dsc.inc
!include QcomPkg/SDMPkg/1000/$(VAR)/Variant.dsc.inc

[LibraryClasses.common]
  UartSettingsLib|QcomPkg/SDMPkg/1000/Settings/UART/UartSettingsLib.inf
  QuadSpiLib|QcomPkg/SDMPkg/Library/QuadSpiLib/QuadSpiLoaderLib.inf
  DALGenLibLoader|QcomPkg/SDMPkg/1000/Settings/DALConfig/DALGenLibLoader_$(VAR).inf
  DALGenLibTools|QcomPkg/SDMPkg/1000/Settings/DALConfig/DALGenLibTools.inf
  DALConfigLoaderLib|QcomPkg/SDMPkg/1000/Settings/DALConfig/DALConfigLoaderLib_$(VAR).inf
  DALConfigToolsLib|QcomPkg/SDMPkg/1000/Settings/DALConfig/DALConfigToolsLib.inf
  IcbTargetLib|QcomPkg/SDMPkg/1000/Library/ICBTargetLib/ICBTargetLib.inf
  IcbCfgLib|QcomPkg/Library/ICBLib/ICBLib.inf
  ChipInfoTargetLib|QcomPkg/SDMPkg/1000/Library/ChipInfoTargetLib/ChipInfoTargetLib.inf
  QusbDciCommonLib|QcomPkg/SDMPkg/1000/Library/QusbDciCommonLib/QusbDciCommonLib.inf
  UsbSharedLib|QcomPkg/SDMPkg/1000/Library/UsbSharedLib/UsbSharedLibLoader.inf
  TsensTargetLib|QcomPkg/SDMPkg/1000/Library/TsensTargetLib/TsensTargetLib.inf
  SpinorTargetLib|QcomPkg/SDMPkg/1000/Library/SpinorTargetLib/loader/SpinorTargetLib.inf
  SdccTargetLib|QcomPkg/SDMPkg/1000/Library/SdccTargetLib/SdccLoaderTargetLib.inf
  SPMILib|QcomPkg/SDMPkg/1000/Library/SPMILib/SPMILib.inf
  UfsTargetBootLib|QcomPkg/SDMPkg/1000/Library/UfsTargetLib/UfsTargetBootLib.inf  
  UfsTargetJtagLib|QcomPkg/SDMPkg/1000/Library/UfsTargetLib/UfsTargetJtagLib.inf
  
[BuildOptions.AARCH64]
  GCC:*_*_AARCH64_ARCHCC_FLAGS  == -mllvm -inline-threshold-multicaller-size=20 -mtune=cortex-a53 -DMDEPKG_NDEBUG -DQCOM_EDK2_PATCH $(FEATURE_FLAGS) -I$(WORKSPACE)/QcomPkg/SDMPkg/Include -I$(WORKSPACE)/QcomPkg/SDMPkg/1000/Include -include $(WORKSPACE)/QcomPkg/SDMPkg/Include/Target_cust.h
  GCC:*_*_AARCH64_ARCHPP_FLAGS  == -DMDEPKG_NDEBUG -DQCOM_EDK2_PATCH $(FEATURE_FLAGS) -I$(WORKSPACE)/QcomPkg/SDMPkg/Include -I$(WORKSPACE)/QcomPkg/SDMPkg/1000/Include -include $(WORKSPACE)/QcomPkg/SDMPkg/Include/Target_cust.h

!if $(PRODMODE) == "PRODMODE"
  GCC:*_*_*_CC_FLAGS = -DPRODMODE
!endif