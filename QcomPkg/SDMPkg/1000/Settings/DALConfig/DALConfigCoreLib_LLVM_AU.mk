# @file DALConfigCoreLib_LLVM.mk
#
#  DAL configuration Tools library Custom Makefile
#
#  Copyright (c) 2016-2019, Qualcomm Technologies, Inc. All rights reserved.
#  Portions Copyright (c) 2009 - 2010, Apple Inc. All rights reserved.
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#=========================================================================


#=======================================README===========================================
# Device Config user adding new xml or updating existing xml need to update
# modify the following variables:
# 1]  INC_DEPS
# 2]  XML_DEPS
#
# XML_DEPS need to be modified if a user adds a config xml in DALConfigLib
# folder of any of its subdirectories. The proper path of the xml file needs
# to be appended to the variable XML_DEPS
#
# INC_DEPS need to be modified if the added config xml file includes (#include directive)
# any header file(s). Proper paths of the header files need to be appended to
# the variable INC_DEPS
# Nothing else should be modified in the file.
#
# Any xxxx.dec Package entries meant to be added in DALConfigCoreLib.inf have to be
# now added to the DALGenLibCore.inf
#========================================================================================


MACRO_NAME              =  core

#
# Following macro i.e. INC_DEPS lists all the header files that
# are included in the xml files. Any new header file added in the
# config xml files need to be appended to the following list
#
INC_DEPS = $(SILICON_PATH)/QcomPkg/Include/api/dal/*.h \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/DALConfig/*.h \
           $(SILICON_PATH)/QcomPkg/Library/PmicLib/framework/inc/pm_dal_prop_ids.h \
           $(SILICON_PATH)/QcomPkg/Include/i2c_config.h \

#
# Following macro i.e. XML_DEPS lists all the xml config files that
# It also enlists any xml files which are #included by other xml files.
# Any new xml file added will need to be appended to the following list.
#
XML_DEPS = $(SILICON_PATH)/QcomPkg/SDMPkg/Settings/DALSystem/$(MACRO_NAME)/dalsystem_$(MACRO_NAME).xml \
           $(SILICON_PATH)/QcomPkg/Settings/ChipInfo/DalChipInfo.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/HWIO/$(MACRO_NAME)/DalHWIO.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/PMIC/AU/pm_config_target_core.xml \
           $(SILICON_PATH)/QcomPkg/Settings/PlatformInfo/PlatformInfo.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/TSENS/tsens_props.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/ADC/internal/core/adc_props.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/ADC/internal/core/vadc_props.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/ICB/core/icbcfg_arb.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/Settings/BAM/bam_config.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/Clock/ClockChipset.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/PCIE/core/pcie_props.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/I2C/core/i2c_devcfg_Sdm1000.xml \
	   $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/SPI/core/spi_devcfg_Sdm1000.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/SMMU/AU/iort.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/Qusb/qusb.xml \
           $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/GPI/core/gpi_props.xml

#=====================USERS NEED NOT EDIT ANYTHING BEYOND THIS LINE========================

INC = @$(MODULE_BUILD_DIR)/../DALGenLibCore_AU/OUTPUT/inc_list.qti

DEVCFG_LIB = $(OUTPUT_DIR)/DALConfigLib.lib
DEVCFG_CONFIG = 1000_xml
IMAGE_CFG_XML = $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/DALConfig/image_cfg.xml
CALLING_MAKE_FILE = $(SILICON_PATH)/QcomPkg/SDMPkg/1000/Settings/DALConfig/DALConfigCoreLib_LLVM_AU.mk
# \
!include <$(SILICON_PATH)/QcomPkg/SDMPkg/Settings/devcfg/devcfg_rules.mk>
# \

# \
!IF 0
  include $(SILICON_PATH)/QcomPkg/SDMPkg/Settings/devcfg/devcfg_rules.mk # \
!ENDIF
