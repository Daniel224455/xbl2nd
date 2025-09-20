# @file DALConfigToolsLib.mk
#
#  DAL configuration Tools library Custom Makefile
#
#  Copyright (c) 2015-2017, Qualcomm Technologies, Inc.
#  All rights reserved.
#  Qualcomm Confidential and Proprietary
#
#
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
# Any xxxx.dec Package entries meant to be added in DALConfigToolsLib.inf have to be
# now added to the DALGenLibTools.inf
#======================================================================================== 



MACRO_NAME              =  tools

#             
# Following macro i.e. INC_DEPS lists all the header files that
# are included in the xml files. Any new header file added in the 
# config xml files need to be appended to the following list     
#
INC_DEPS = $(WORKSPACE)/QcomPkg/Include/api/dal/*.h \
           $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/DALConfig/*.h \
 
#             
# Following macro i.e. XML_DEPS lists all the xml config files that
# It also enlists any xml files which are #included by other xml files.
# Any new xml file added will need to be appended to the following list.    
#                           
# __FIX__SDM855:  uncomment usb xml when fixed
XML_DEPS = $(WORKSPACE)/QcomPkg/SDMPkg/Settings/DALSystem/$(MACRO_NAME)/dalsystem_$(MACRO_NAME).xml \
           

#=====================USERS NEED NOT EDIT ANYTHING BEYOND THIS LINE======================== 



 
#
# Build Macro
#
OBJECT_FILES = $(OUTPUT_DIR)/DALConfig_$(MACRO_NAME).obj

    
         
INC = @$(MODULE_BUILD_DIR)/../DALGenLibTools/OUTPUT/inc_list.qti

DEVCFG_LIB = $(OUTPUT_DIR)/DALConfigToolsLib.lib
DEVCFG_CONFIG = 1000_xml
IMAGE_CFG_XML = $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/DALConfig/image_cfg.xml
CALLING_MAKE_FILE = $(WORKSPACE)/QcomPkg/SDMPkg/1000/Settings/DALConfig/DALConfigToolsLib.mk
# \
!include <$(WORKSPACE)/QcomPkg/SDMPkg/Settings/devcfg/devcfg_rules.mk>
# \

# \
!IF 0
  include $(WORKSPACE)/QcomPkg/SDMPkg/Settings/devcfg/devcfg_rules.mk # \
!ENDIF
