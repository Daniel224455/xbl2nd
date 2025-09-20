# @file Apriori.fdf.inc
# Apriori for WP SDM1000
#
# Copyright (c) 2014-2019 Qualcomm Technologies, Inc. All rights reserved.
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
#
#==============================================================================
#                              EDIT HISTORY
#
#
# when       who     what, where, why
# --------   ---     ----------------------------------------------------------
# 11/12/19   owen    Added WLAN(Hastings) power up sequence
# 07/24/19   dp      Moved TpmDxe after ChargerExDxe
# 04/05/19   ssumit  Added I2CKeyboard after I2C Dxe
# 03/14/19   shl     Moved GpiDxe before TzDxe 
# 01/22/19   ml      Re-ordered DisplayDxe after VariableDxe
# 01/22/19   kpa     switch to DiskIo and PartitionDxe from MdeModulePkg
# 01/09/19   bng     Added SPIDxe
# 10/23/18   tselvam re-ordered PciHostBridgeDxe after AcpiPlatformDxe      
# 09/19/18   pr      Enabled TpmDxe
# 08/06/18   shl     Revert TpmDxe
# 08/28/18   daisond Added DDRInfoDxe  before DisplayDxe
# 08/06/18   shl     Added TpmDxe
# 08/03/18   ivy     Move PciHostBridgeDxe after ChargerExDxe
# 07/31/18   kpa     Temp revert DisplayDxe load order to resolve HLOS SMMU issue
# 07/25/18   kpa     Ensure Storage Dxe's are loaded before TZDxe, Display
# 05/25/18   prema   Capsule update related changes
# 07/17/18   ml      Added comments for DisplayDxe
# 05/24/18   unr     Moved I2C before Pmic
# 05/18/18   jp      Added CipherDxe
# 05/03/18   pranavj Ported SHMBridge changes to Poipu
# 03/29/18   rs      Move SdccDxe prior to DisplayDxe
# 03/20/18   kpa     Move FvSimpleFileSystemDxe prior to DisplayDxe
# 02/20/18   dc      Added UsbPwrCtrlDxe
# 12/01/17   jt      Update for NVMe
# 03/23/17   jp      Enabled Hash and RNG
# 10/26/16   pr      Moved SecurityDxe after NpaDxe for Hash calculation during ExitBS
# 06/28/16   na      Move SPMIDxe between VariableDxe and CrashDumpDxe
# 06/20/16   sm      Moved QcomChargerDxe and ADCDxe before USB drivers.
# 05/23/16   ck      Adding PMIC Charger App support
# 05/03/16   ck      Adding USB Init Protocol
# 03/25/16   va      Adding ChargerDxeWp for Wp
# 03/03/16   va      updating PmicDxe for Wp
# 03/04/16   bh      Move Fat/English/FVSimple under UFS/Sdcc
# 10/28/15   jk      Added QdssDxe
# 07/29/15   ao      move CrashDumpDxe driver to load after variableDxe
# 06/17/15   al      Move ChargerDxe after display
# 03/17/15   vk      Enable security
# 02/15/15   vk      Fix reset, add RPMB apps
# 02/05/15   rli     Replace SmdDxe with GLinkDxe
# 11/11/14   bh      Disable SetupBrowser, remove old BdsDxe
# 10/14/14   jb      Disable prebuilt security drivers until availble
#                    Use ArmGicDriver
# 10/07/14   vk      Use Qualcomm DiskIo
# 09/07/14   cpaulo  Added Prio for GicDxe and TimerDxe
# 08/18/14   bn      Renamed MMCHSDxe to SdccDxe
# 07/09/14   vk      Initial revision
#
#==============================================================================

########################### APRIORI Start ####################################################
## Load order for drivers ##
## NOTE: After the APRIORI End section is where drivers must be added in order for them to
##       be included in the FD. The APRIORI just dictates a load order regardless of whether
##       the drivers are actually included in the FD or not

  APRIORI DXE {
        INF MdeModulePkg/Core/Dxe/DxeMain.inf

#
#    Arch Protocols
#
#    EfiSecurityArchProtocolGuid,
#    EfiCpuArchProtocolGuid,
#    EfiMetronomeArchProtocolGuid,
#    EfiTimerArchProtocolGuid,
#    EfiBdsArchProtocolGuid,
#    EfiWatchdogTimerArchProtocolGuid,
#    EfiRuntimeArchProtocolGuid,
#    EfiVariableArchProtocolGuid,
#    EfiVariableWriteArchProtocolGuid,
#    EfiCapsuleArchProtocolGuid,
#    EfiMonotonicCounterArchProtocolGuid,
#    EfiResetArchProtocolGuid,
#    EfiRealTimeClockArchProtocolGuid,
#

        INF QcomPkg/Drivers/EnvDxe/EnvDxe.inf

# Report Status Code Handler and Router
        INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
        INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf


#  Arch Protocol :  EfiRuntimeArchProtocolGuid,
        INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

#  Arch Protocol :  EfiCpuArchProtocolGuid,
        INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
        INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

#  Arch Protocol :  EfiTimerArchProtocolGuid,
        INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

        # NOTE: DAL has dependency on SMEM
        INF QcomPkg/Drivers/SmemDxe/SmemDxe.inf
        INF QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf

        INF QcomPkg/Drivers/HWIODxe/HWIODxe.inf
        INF QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
        INF QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf

        INF QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf

#  Arch Protocol :  EfiMetronomeArchProtocolGuid,
        INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

        INF QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxe.inf		
        INF QcomPkg/Drivers/TzDxe/ScmDxe.inf

        INF QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
        INF QcomPkg/Drivers/ULogDxe/ULogDxe.inf
        INF QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
        INF QcomPkg/Drivers/NpaDxe/NpaDxe.inf
        INF QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
        INF QcomPkg/Drivers/PdcDxe/PdcDxe.inf
        INF QcomPkg/Drivers/ClockDxe/ClockDxe.inf

        INF QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
        INF QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
        INF QcomPkg/Drivers/I2CDxe/I2CDxe.inf
        INF QcomPkg/Drivers/I2CKeyboard/I2CKeyboard.inf
# DDRInfo Protocol		
        INF QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
		
#  Arch Protocol :  EfiResetArchProtocolGuid,
        INF QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf				
        INF QcomPkg/Drivers/PmicDxe/PmicDxeWp.inf

        # move these below display once the storage dependency gets addressed
        INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
        INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf      

		
        INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
        INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf        

        INF QcomPkg/Drivers/SdccDxe/SdccDxe.inf
        INF QcomPkg/Drivers/UFSDxe/UFSDxe.inf
        INF QcomPkg/Drivers/PILDxe/PILDxe.inf
        INF QcomPkg/Drivers/CPRDxe/CPRDxe.inf

#  Arch Protocol :  EfiWatchdogTimerArchProtocolGuid,
        INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

#  Arch Protocol :  EfiSecurityArchProtocolGuid,
        INF QcomPkg/Drivers/SecurityDxe/SecurityDxe.inf

        INF QcomPkg/Drivers/SpiNorDxe/SpiNorDxe.inf
        INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf

        INF QcomPkg/Drivers/WconnDxe/WconnDxe.inf

        INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
        INF FatPkg/EnhancedFatDxe/Fat.inf

        # DisplayDxe depends on storage, filesystem, variable services
        INF QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf

# GpiDxe must be loaded before TpmDxe, otherwise it will not work 
        INF QcomPkg/Drivers/GpiDxe/GpiDxe.inf      
        INF QcomPkg/Drivers/TzDxe/TzDxe.inf        

#  Arch Protocol :  EfiVariableArchProtocolGuid,
#  Arch Protocol :  EfiVariableWriteArchProtocolGuid,
        INF QcomPkg/Drivers/VariableDxe/VariableDxe.inf
#        INF MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf

#  HW wdog driver, depends on variabledxe
        INF QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

#QDSS Driver (Debugtrace)
        #INF QcomPkg/Drivers/QdssDxe/QdssDxe.inf

        #
        # Crashdump Driver
        #
	INF QcomPkg/Drivers/CrashDumpDxe/CrashDumpDxe.inf
		# Raw File System Protocol: EfiRawFileSystemProtocolGuid
 	 	INF QcomPkg/Drivers/RawFileSystemDxe/RawFileSystemDxe.inf

        INF QcomPkg/Drivers/DppDxe/DppDxe.inf

#  Arch Protocol :  EfiMonotonicCounterArchProtocolGuid,
        INF QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf


#  Arch Protocol :  EfiRealTimeClockArchProtocolGuid,
        INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf

        INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
        INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

#  Arch Protocol :  EfiBdsArchProtocolGuid,
        # HIIdb and Fonts are BDS dependencies
        INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
        #INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
        INF QcomPkg/Drivers/QcomBds/QcomBds.inf

        INF QcomPkg/Drivers/SPIDxe/SPIDxe.inf


        #
        # Debug Protocol
        #
        #INF QcomPkg/Drivers/DebugPortDxe/DebugPortDxe.inf
        INF QcomPkg/Drivers/SerialDxe/SerialDxe.inf


        #
        #  Moved ChargerDxe/AdcDxe/ButtonsDxe above DisplayDxe
        #
        INF QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
        #INF QcomPkg/Drivers/TsFwUpgradeDxe/TsFwUpgradeDxe.inf
        INF QcomPkg/Drivers/TsensDxe/TsensDxe.inf

        INF QcomPkg/Drivers/AdcDxe/AdcDxe.inf
        INF QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
        INF QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
        INF QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeWp.inf
        INF QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
        #
        #  Moved TpmDxe after ChargerDxe to address Off Mode Charging issue
        #
        INF QcomPkg/Drivers/TrEEDxe/TpmDxe.inf

        #
        # Adding Limits DxE
        #
        INF QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf

        INF QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
        INF QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
        INF QcomPkg/Drivers/XhciDxe/XhciDxe.inf
        INF QcomPkg/Drivers/UsbBusDxe/UsbBusDxe.inf
        INF QcomPkg/Drivers/UsbKbDxe/UsbKbDxe.inf
        INF QcomPkg/Drivers/UsbMassStorageDxe/UsbMassStorageDxe.inf
        INF QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
        INF QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
        INF QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
        INF QcomPkg/Drivers/UsbfnChgDxe/UsbfnChgDxe.inf

		#
		#     Critical modules until here,
		#
		#     Non critical parts follow from here
		#

        INF QcomPkg/Drivers/SmbiosDxe/SmbiosDxe.inf
        INF MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
        INF QcomPkg/Drivers/AcpiPlatformDxe/AcpiPlatformDxe.inf

        # Pci HostBridge Dxe
        INF QcomPkg/Drivers/PciHostBridgeDxe/PciHostBridgeDxe.inf

        INF QcomPkg/Drivers/HashDxe/HashDxe.inf
        INF QcomPkg/Drivers/CipherDxe/CipherDxe.inf
        INF QcomPkg/Drivers/RNGDxe/RngDxe.inf
        INF QcomPkg/Drivers/TrEEDxe/TrEEDxe.inf

	    #
	    # ACPI updating Driver
	    #
	    #INF QcomPkg/Drivers/WinAcpiUpdate/WinAcpiUpdate.inf

        INF QcomPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
        INF QcomPkg/Drivers/AdapterInformationDxe/AdapterInformationDxe.inf

        # Console
        # HIIdb and Fonts are dependencies for these and should be loaded before
        INF QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf
        INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
        INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
        INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

#  Arch Protocol :  EfiCapsuleArchProtocolGuid,
        #INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
        INF QcomPkg/Drivers/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf

  }

#
########################### APRIORI End ####################################################
#

