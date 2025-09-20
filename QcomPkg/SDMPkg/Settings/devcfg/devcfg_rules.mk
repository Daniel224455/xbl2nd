THISFILE = $(SILICON_PATH)/QcomPkg/SDMPkg/Settings/devcfg/devcfg_rules.mk

OBJECT_FILES_LIST = $(OUTPUT_DIR)/object_files.lst
DEVCFG_O = $(OUTPUT_DIR)/DALConfig_$(MACRO_NAME).obj
DEVCFG_C = $(OUTPUT_DIR)/devcfg.c
DEVCFG_DATA_O = $(OUTPUT_DIR)/devcfg_data.o
DEVCFG_DATA_C = $(OUTPUT_DIR)/devcfg_data.c
DEVCFG_XML = $(OUTPUT_DIR)/DALConfig_$(MACRO_NAME).xml
STRUCTPTR_H = $(OUTPUT_DIR)/structptrs.h
DEVCFG_BLOB_TABLE_C = $(OUTPUT_DIR)/devcfgBlobTable.c
DEVCFG_BLOB_TABLE_O = $(OUTPUT_DIR)/devcfgBlobTable.o
DALMOD_C = $(OUTPUT_DIR)/DAL_modules.c
DALMOD_O = $(OUTPUT_DIR)/DAL_modules.o
DEVCFG_C_DEFINES = -DDEVCFG__STRUCT_SIZE -DDEVCFG__PROPS_MEMORY_OPTIMISE -DDEVCFG_BOOT
DEVCFG_SCRIPTS = $(SILICON_PATH)/QcomPkg/Tools/devcfg/scripts
DEVCFG_OBJECTS = $(DEVCFG_O) $(DEVCFG_BLOB_TABLE_O) $(DEVCFG_DATA_O) $(DALMOD_O)

GLOBAL_DEPS = $(THISFILE) $(XML_DEPS) $(XML_XBLCONFIG_DEPS) $(INC_DEPS) $(IMAGE_CFG_XML) $(DEVCFG_SCRIPTS)/image_config.py $(DEVCFG_SCRIPTS)/propgen.py $(CALLING_MAKE_FILE)

# \
XML_INPUT_LIST = -i $(XML_DEPS:  = -i )
# \
# \
!IF 0
  XML_INPUT_LIST = $(addprefix -i ,$(XML_DEPS)) # \
!ENDIF

# \
!IFDEF XML_XBLCONFIG_DEPS # \
XML_XBLCONFIG_INPUT_LIST = -i $(XML_XBLCONFIG_DEPS:  = -i ) # \
!ENDIF # \
# \
# \
!IF 0
  XML_XBLCONFIG_INPUT_LIST = $(addprefix -i ,$(XML_XBLCONFIG_DEPS)) # \
!ENDIF

DALMOD_XMLS_LIST = $(XML_INPUT_LIST) $(XML_XBLCONFIG_INPUT_LIST)

#======================================================================
#
# Individual Object Build Targets
#
$(DEVCFG_LIB) : $(DEVCFG_OBJECTS) $(GLOBAL_DEPS)
	"$(SLINK)" -D -cr $(DEVCFG_LIB) $(SLINK_FLAGS) $(DEVCFG_OBJECTS) $(C_FILES)

$(DEVCFG_DATA_O) : $(DEVCFG_C) $(STRUCTPTR_H) $(MAKE_FILE) $(COMMON_DEPS) $(GLOBAL_DEPS)
	"$(CC)" $(CC_FLAGS) $(DEVCFG_C_DEFINES) -include $(STRUCTPTR_H) -o $(DEVCFG_DATA_O) $(INC) $(DEVCFG_DATA_C)

$(DEVCFG_O) : $(DEVCFG_C) $(STRUCTPTR_H) $(MAKE_FILE) $(COMMON_DEPS) $(GLOBAL_DEPS)
	"$(CC)" $(CC_FLAGS) $(DEVCFG_C_DEFINES) -include $(STRUCTPTR_H) -o $(DEVCFG_O) $(INC) $(DEVCFG_C)

$(DEVCFG_C) : $(DEVCFG_XML) $(GLOBAL_DEPS)
	( python $(DEVCFG_SCRIPTS)/propgen.py -t $(DEVCFG_CONFIG) -i $(DEVCFG_XML) -c $(DEVCFG_C) -e $(DEVCFG_DATA_C) -g $(IMAGE_CFG_XML) )

$(DEVCFG_XML) : $(GLOBAL_DEPS)
	( python $(DEVCFG_SCRIPTS)/propgen.py -j $(DEVCFG_XML) $(XML_INPUT_LIST))

$(STRUCTPTR_H) : $(DEVCFG_XML) $(GLOBAL_DEPS)
	( python $(DEVCFG_SCRIPTS)/propgen.py -t $(DEVCFG_CONFIG) -u $(STRUCTPTR_H) -i $(DEVCFG_XML) -g $(IMAGE_CFG_XML) )

$(DEVCFG_BLOB_TABLE_O) : $(DEVCFG_BLOB_TABLE_C) $(GLOBAL_DEPS)
	"$(CC)" $(CC_FLAGS) $(DEVCFG_C_DEFINES) -o $(DEVCFG_BLOB_TABLE_O) $(INC) $(DEVCFG_BLOB_TABLE_C)

$(DEVCFG_BLOB_TABLE_C) : $(GLOBAL_DEPS)
	( python $(DEVCFG_SCRIPTS)/image_config.py $(IMAGE_CFG_XML) $(DEVCFG_BLOB_TABLE_C) )

$(DALMOD_C) : $(GLOBAL_DEPS)
	( python $(DEVCFG_SCRIPTS)/propgen.py -t $(DEVCFG_CONFIG) $(DALMOD_XMLS_LIST) --ModDirFile=$(DALMOD_C) -g $(IMAGE_CFG_XML) )

$(DALMOD_O) : $(DALMOD_C) $(GLOBAL_DEPS)
	"$(CC)" $(CC_FLAGS) $(DEVCFG_C_DEFINES) -o $(DALMOD_O) $(INC) $(DALMOD_C)

# List all targets as dependencies the the psuedo target to force rebuild of everything
all: $(DEVCFG_LIB) $(DEVCFG_O) $(DEVCFG_DATA_O) $(DEVCFG_O) $(DEVCFG_C) $(DEVCFG_XML) $(STRUCTPTR_H) $(DEVCFG_BLOB_TABLE_O) $(DEVCFG_BLOB_TABLE_C) $(DALMOD_C) $(DALMOD_O)

#
# Build Flash Device Image
#
gen_fds:
	@"$(MAKE)" $(MAKE_FLAGS) -f $(BUILD_DIR)\Makefile fds
	@cd $(MODULE_BUILD_DIR)

#
# Target to update the FD
#
fds: mbuild gen_fds


#
# clean all intermediate files
#
clean:
	if exist $(OUTPUT_DIR) $(RD) $(OUTPUT_DIR)


#
# clean all generated files
#
cleanall:
	if exist $(DEBUG_DIR) $(RD) $(DEBUG_DIR)
	if exist $(OUTPUT_DIR) $(RD) $(OUTPUT_DIR)
	$(RM) *.pdb *.idb > NUL 2>&1
	$(RM) $(BIN_DIR)\$(MODULE_NAME).efi

#
# clean all dependent libraries built
#
cleanlib:
	@cd $(MODULE_BUILD_DIR)
