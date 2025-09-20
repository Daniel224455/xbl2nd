#
# Build Macro
#
TARGET_ID = 1000
CHIP_ID = $(TARGET_ID)

CPR_CFG_SCRIPT = $(SILICON_PATH)/QcomPkg/Library/CPRLib/tools/cpr_cfg.py
CPR_CONFIG_DIR = $(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Settings/CPR
CPR_FUSES_DIR  = $(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/target/$(TARGET_ID)

#=======================================================================
#Include the dependency file for DALConfig_<MACRO>.c file
#If any files #included by the DALConfig C file are changed,
#Then DALConfig_<MACRO>.obj/lib and the dependency file are rebuilt.
#======================================================================


OBJ =  \
    $(OUTPUT_DIR)/Library/CPRLib/common/hal/v3/src/cpr_hal.obj \
    $(OUTPUT_DIR)/Library/CPRLib/common/src/cpr.obj \
    $(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_checksum.obj \
    $(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_measurements.obj \
    $(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_rail.obj \
    $(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_smem.obj \
    $(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_smem_vp.obj \
    $(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_utils.obj \
    $(OUTPUT_DIR)/Library/CPRLib/image/core/src/cpr_image.obj \
    $(OUTPUT_DIR)/Library/CPRLib/image/core/src/cpr_image_logs.obj \
    $(OUTPUT_DIR)/image/core/target/$(TARGET_ID)/cpr_image_target_init.obj \
    $(OUTPUT_DIR)/image/core/target/$(TARGET_ID)/cpr_image_target.obj


INC =  \
    -I$(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/target/$(TARGET_ID) \
    -I$(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/target/$(TARGET_ID)/hwio \
    -I$(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/image/core/target/$(TARGET_ID) \
    -I$(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Settings/CPR \
    -I$(SILICON_PATH)/QcomPkg/Library/CPRLib/image/core/inc \
    -I$(SILICON_PATH)/QcomPkg/Library/CPRLib/common/inc \
    -I$(SILICON_PATH)/QcomPkg/Library/CPRLib/common/hal/inc \
    -I$(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib \
    -I$(DEBUG_DIR) \
    -I$(EDK2_CORE_PATH)/MdePkg \
    -I$(EDK2_CORE_PATH)/MdePkg/Include \
    -I$(EDK2_CORE_PATH)/MdePkg/Include/AArch64 \
    -I$(EDK2_CORE_PATH)/MdeModulePkg \
    -I$(EDK2_CORE_PATH)/MdeModulePkg/Include \
    -I$(SILICON_PATH)/QcomPkg \
    -I$(SILICON_PATH)/QcomPkg/Include \
    -I$(SILICON_PATH)/QcomPkg/Include/Library \
    -I$(SILICON_PATH)/QcomPkg/Include/api/dal \
    -I$(SILICON_PATH)/QcomPkg/Include/api/kernel/SMMU \
    -I$(SILICON_PATH)/QcomPkg/Library/MuslLib/musl/include \
    -I$(SILICON_PATH)/QcomPkg/Library/MuslLib/musl/arch/arm \
    -I$(EDK2_CORE_PATH)/EmbeddedPkg \
    -I$(EDK2_CORE_PATH)/EmbeddedPkg/Include \
    -I$(SILICON_PATH)/QcomPkg/Include/api/boot \
    -I$(SILICON_PATH)/QcomPkg/Include/api/boot/qfprom \
    -I$(SILICON_PATH)/QcomPkg/Include/api/systemdrivers \
    -I$(SILICON_PATH)/QcomPkg/Include/api/pmic/pm \
    -I$(SILICON_PATH)/QcomPkg/Include/api/buses \
    -I$(SILICON_PATH)/QcomPkg/Include/api/securemsm/secboot \
    -I$(SILICON_PATH)/QcomPkg/Include/api/securemsm/seccfg \
    -I$(SILICON_PATH)/QcomPkg/Include/api/securemsm/secimgauth \
    -I$(SILICON_PATH)/QcomPkg/Include/api/securemsm \
    -I$(SILICON_PATH)/QcomPkg/Include/api/storage \
    -I$(SILICON_PATH)/QcomPkg/Include/api/systemdebug \
    -I$(SILICON_PATH)/QcomPkg/Include/api/wiredconnectivity \
    -I$(SILICON_PATH)/QcomPkg/Include/api/services \
    -I$(SILICON_PATH)/QcomPkg/Include/api/debugtools \
    -I$(SILICON_PATH)/QcomPkg/Include/api/hwengines \
    -I$(SILICON_PATH)/QcomPkg/Include/api/mproc \
    -I$(SILICON_PATH)/QcomPkg/Include/api/kernel/libstd \
    -I$(SILICON_PATH)/QcomPkg/Include/api/kernel/devcfg \
    -I$(SILICON_PATH)/QcomPkg/Include/XBLLoader


#
# Default target, which will build dependent libraries in addition to source files
#
all: gen_libs $(OUTPUT_DIR)/CPRTargetLib.lib

#
# Phony target which is used to force executing commands for a target
#
force_build:
	-@

#
# Target to update the FD
#

fds: mbuild gen_fds

strdefs:
	-@$(CP) $(DEBUG_DIR)/AutoGen.h $(DEBUG_DIR)/$(MODULE_NAME)StrDefs.h

#
# GenLibsTarget
#
gen_libs:
	@cd $(MODULE_BUILD_DIR)

#
# Build Flash Device Image
#
gen_fds:
	@"$(MAKE)" $(MAKE_FLAGS) -f $(BUILD_DIR)/Makefile fds
	@cd $(MODULE_BUILD_DIR)

#
# Individual Object Build Targets
#
$(OUTPUT_DIR)/CPRTargetLib.lib : $(OBJ)
	"$(SLINK)" -D -cr $@ $(SLINK_FLAGS) $(OBJ)

$(OUTPUT_DIR)/image/core/target/$(TARGET_ID)/cpr_image_target_init.obj : $(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/image/core/target/$(TARGET_ID)/cpr_image_target_init.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/image/core/target/$(TARGET_ID)/cpr_image_target_init.c

$(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_smem.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_smem.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_smem.c

$(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_smem_vp.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_smem_vp.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_smem_vp.c

$(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_rail.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_rail.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_rail.c

$(OUTPUT_DIR)/Library/CPRLib/common/hal/v3/src/cpr_hal.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/hal/v3/src/cpr_hal.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/hal/v3/src/cpr_hal.c

$(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_checksum.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_checksum.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_checksum.c

$(OUTPUT_DIR)/Library/CPRLib/common/src/cpr.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr.c

$(OUTPUT_DIR)/image/core/target/$(TARGET_ID)/cpr_image_target.obj : $(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/image/core/target/$(TARGET_ID)/cpr_image_target.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/SDMPkg/$(CHIP_ID)/Library/CPRTargetLib/image/core/target/$(TARGET_ID)/cpr_image_target.c

$(OUTPUT_DIR)/Library/CPRLib/image/core/src/cpr_image.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/image/core/src/cpr_image.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/image/core/src/cpr_image.c

$(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_measurements.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_measurements.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_measurements.c

$(OUTPUT_DIR)/Library/CPRLib/image/core/src/cpr_image_logs.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/image/core/src/cpr_image_logs.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/image/core/src/cpr_image_logs.c

$(OUTPUT_DIR)/Library/CPRLib/common/src/cpr_utils.obj : $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_utils.c
	"$(CC)" $(CC_FLAGS) -o $@ $(INC) $(SILICON_PATH)/QcomPkg/Library/CPRLib/common/src/cpr_utils.c


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
	$(RM) $(BIN_DIR)/$(MODULE_NAME).efi

#
# clean all dependent libraries built
#
cleanlib:
	@cd $(MODULE_BUILD_DIR)

