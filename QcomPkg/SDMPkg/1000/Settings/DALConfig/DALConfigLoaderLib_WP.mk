# @file DALConfigLoaderLib_WP.mk
#
#  DAL configuration Tools library Custom Makefile
#
#  Copyright (c) 2015-2019, Qualcomm Technologies, Inc.
#  All rights reserved.
#  Qualcomm Confidential and Proprietary

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
# Any xxxx.dec Package entries meant to be added in DALConfigLoaderLib.inf have to be
# now added to the DALGenLibLoader.inf
#========================================================================================


MACRO_NAME = loader

#
# Following macro i.e. INC_DEPS lists all the header files that
# are included in the xml files. Any new header file added in the
# config xml files need to be appended to the following list
#
INC_DEPS = $(WORKSPACE)/QcomPkg/Include/api/dal/*.h \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/DALConfig/*.h \
           $(WORKSPACE)/QcomPkg/Library/PmicLib/framework/inc/pm_dal_prop_ids.h \
           $(WORKSPACE)/QcomPkg/Include/i2c_config.h \
	   $(WORKSPACE)/QcomPkg/Include/spi_config.h \
           $(WORKSPACE)/QcomPkg/SDMPkg/Include/GPIODevCfg.h \

#
# Following macro i.e. XML_DEPS lists all the xml config files that
# It also enlists any xml files which are #included by other xml files.
# Any new xml file added will need to be appended to the following list.
#
# __FIX__SDM855:  uncomment driver xmls when corresponding drivers are ported
XML_DEPS = $(WORKSPACE)/QcomPkg/SDMPkg/Settings/DALSystem/$(MACRO_NAME)/dalsystem_$(MACRO_NAME).xml \
           $(WORKSPACE)/QcomPkg/Settings/ChipInfo/DalChipInfo.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/PMIC/pm_config_target_loader.xml \
           $(WORKSPACE)/QcomPkg/Settings/PlatformInfo/PlatformInfo.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/TSENS/tsens_props.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/Settings/BootTempCheck/boot_temp_check_props.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/ADC/internal/loader/adc_props.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/ADC/internal/loader/vadc_props.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/Settings/BAM/bam_config.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/Settings/UFS/ufs_config.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/SPINOR/spinor_access.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/ICB/loader/icbcfg_boot.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/Settings/SMEM/smem_xbl_loader.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/TLMM/loader/TLMMChipset.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/I2C/loader/i2c_devcfg_Sdm1000.xml \
	   $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/SPI/loader/spi_devcfg_Sdm1000.xml \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/Qusb/qusb.xml

#=====================USERS NEED NOT EDIT ANYTHING BEYOND THIS LINE========================

OBJECT_FILES = $(OUTPUT_DIR)/DALConfig_$(MACRO_NAME).obj

INC = @$(MODULE_BUILD_DIR)/../DALGenLibLoader_WP/OUTPUT/inc_list.qti

DEVCFG_LIB = $(OUTPUT_DIR)/DALConfigLoaderLib.lib
DEVCFG_CONFIG = 1000_xml
IMAGE_CFG_XML = $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/DALConfig/image_cfg.xml
CALLING_MAKE_FILE = $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/DALConfig/DALConfigLoaderLib_WP.mk

# \
!include <$(WORKSPACE)/QcomPkg/SDMPkg/Settings/devcfg/devcfg_rules.mk>
# \

# \
!IF 0
  include $(WORKSPACE)/QcomPkg/SDMPkg/Settings/devcfg/devcfg_rules.mk # \
!ENDIF
