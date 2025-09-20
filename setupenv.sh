#!/bin/bash

sudo apt install -y \
  pip \
  git \
  mono-devel \
  build-essential \
  lld \
  uuid-dev \
  nasm \
  gcc-aarch64-linux-gnu \
  python3 \
  python3-git \
  python3-pip \
  gettext \
  locales \
  gnupg \
  ca-certificates \
  python3-venv \
  git \
  git-core \
  clang \
  llvm \
  curl \
  lld \
  iasl \
  python \
  python3-distutils \
  python3-apt \
  libncurses5 \
  exuberant-ctags \
  libudev-dev \
  libpci-dev \
  libiberty-dev \
  autoconf \
  libncurses-dev \
  bison \
  flex \
  libssl-dev \
  libelf-dev \
  bc \
  dwarves

if [ ! -d "/pkg/qct/software/llvm/release/arm/4.0.2" ]; then
    echo "Setting up SnapdragonLLVM 4.0.2"
    sudo mkdir /pkg
    sudo mkdir -p /pkg/qct/software/llvm/release/arm/4.0.2
    sudo cp -r $(pwd)/SDLLVM2/* /pkg/qct/software/llvm/release/arm/4.0.2
fi

if [ ! -d "/pkg/qct/software/llvm/release/arm/10.0.4" ]; then
    echo "Setting up SnapdragonLLVM 10.0.4"
    sudo mkdir /pkg
    sudo mkdir -p /pkg/qct/software/llvm/release/arm/10.0.4
    sudo cp -r $(pwd)/SDLLVM/* /pkg/qct/software/llvm/release/arm/10.0.4
fi

if [ ! -d "/pkg/qct/software/arm/linaro-toolchain/aarch64-none-elf/4.9-2014.07" ]; then
    echo "Setting up the Linaro toolchain"
    sudo mkdir /pkg
    sudo mkdir -p /pkg/qct/software/arm/linaro-toolchain/aarch64-none-elf/4.9-2014.07
    wget https://releases.linaro.org/archive/14.07/components/toolchain/binaries/gcc-linaro-aarch64-none-elf-4.9-2014.07_linux.tar.xz
    sudo tar -xf "$(pwd)/gcc-linaro-aarch64-none-elf-4.9-2014.07_linux.tar.xz" -C /pkg/qct/software/arm/linaro-toolchain/aarch64-none-elf/4.9-2014.07
    sudo rm gcc-linaro-aarch64-none-elf-4.9-2014.07_linux.tar.xz
fi

if [ ! -f "/pkg/qct/software/boottools/dtc" ]; then
    echo "Linking DeviceTreeCompiler ..."
    sudo mkdir -p /pkg/qct/software/boottools
    sudo ln -sf /usr/bin/dtc /pkg/qct/software/boottools/dtc
fi

if [ "$(stat -c "%a" /pkg)" != "777" ]; then
    sudo chmod -R 777 /pkg
fi

	git update-index --assume-unchanged   QcomPkg/Tools/MapParser.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/Parser_CLANG.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/XBLConfig/commons.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/XBLConfig/elf_gen_tools.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/ext/ordereddict/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/ext/ordereddict/ordereddict.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/plugin/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/plugin/encrypted_key_provider/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/plugin/encrypted_key_provider/encrypted_key_provider_oem.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/plugin/signer/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/plugin/signer/hsm_oem.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/builder/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/builder/__secfile__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/builder/auto_gen_policy_parser.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/core/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/core/plugin.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/crypto_cbc.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/crypto_ccm.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/ecies.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/factory.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/m2crypto.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/discovery/openssl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/aes_cbc/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/aes_cbc/openssl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/aes_cbc_cts/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/aes_cbc_cts/crypto_cbc.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/aes_ccm/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/aes_ccm/crypto_ccm.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/cert/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/cert/openssl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/ecdsa/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/ecdsa/openssl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/eciesad/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/eciesad/ecies.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/rsa/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/rsa/asn1.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/rsa/openssl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/utils/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/crypto/functions/utils/openssl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/data_provisioning/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/data_provisioning/data_prov.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/data_provisioning/xmltodict.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/format.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/header/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/header/format.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/header/ident.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/header/machine.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/header/type.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/header/version.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/section/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/section/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/section/flags.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/section/format.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/section/type.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/segment/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/segment/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/segment/flags.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/segment/format.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/segment/license_manager.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/segment/multi_image.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/elf/segment/type.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/utils.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v3/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v3/headers.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v3/parsegen_mbn.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v3/utils.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v3_encdec/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v3_encdec/parsegen_mbn.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v3_encdec/utils.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v5/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v5/headers.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v5/parsegen_mbn.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v5/utils.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v6/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v6/headers.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v6/parsegen_mbn.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/mbn/versions/v6/utils.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/pmbl/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/pmbl/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/pmbl/format.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/pmbl/magic.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/pmbl/ota.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/parsegen/pmbl/preamble.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/rpc/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/rpc/http/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_attribute.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_config.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_config_overrides.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_data.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_logging.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_misc.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_path.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_platform.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_process.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/c_rule.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/base_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/boolean_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/data_detail.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/dict_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/hex16_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/hex32_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/hex64_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/hex_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/integer_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/list_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/path_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/string_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/datautils/tuple_handler.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/common/utils/struct_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/advanced_defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/api/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/builder/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/builder/__secfile__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/builder/sectools_builder_core.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/cfgparser/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/cfgparser/auto_gen_obj_config.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/cfgparser/auto_gen_xml_config.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/cfgparser/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/cfgparser/rule.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/data_provision_enquirer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/base/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/base/decryptor.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/base/encdec.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/base/encryption_parameters.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/base/encryptor.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/__secfile__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/auto_gen_ssd_xml_config.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/decryptor.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/encdec.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/encryption_parameters.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/encryptor.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/ssd/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/ssd/__secfile__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/ssd/gen_ssd_bin.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/ssd/ssd/key_config_parser.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/base_encrypted_key_provider.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/decryptor.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encdec.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encrypted_key_provider.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encrypted_key_provider_mgr.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encryption_parameters.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encryption_params_info_hdr.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encryption_params_section_body.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encryption_params_section_hdr.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/unified/encryptor.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/encryption_service/utility_functions.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/hasher/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/hasher/hasher.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/imageinfo/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/imageinfo/imageinfo_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/imageinfo/imageinfo_utils.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/imageinfo/secimageinfo.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/key_service_enquirer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/base_encdec.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/bin.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/bin_to_sign.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/config/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/config/auto_gen_obj_config.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/config/auto_gen_xml_config.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/config/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/config/parser.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_base/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_base/elf_v3_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_base/mbn_v3_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_encdec/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_encdec/elf_v3_encdec.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_encdec/encdec.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_encdec/mbn_v3_encdec.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_licmngr/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v3_licmngr/elf_v3_licmngr.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v5/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v5/elf_v5.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v5/mbn_v5.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v6/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v6/elf_v6.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v6/mbn_v6.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v6_multi_image/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_mbn_v6_multi_image/elf_v6_multi_image.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_support/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_support/elf_delegate.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/elf_support/elf_vars.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/ewm/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/ewm/format.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/ewm/header.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/ewm/segment.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/get_parser.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/hash_to_sign.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/mbn.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/mbn_tools.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/parsegen/pmbl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/secimage/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/secimage/secimage.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/secimage/secimage_cfgparser.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/secimage/secimage_core.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/secimage_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/secpolicy.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/base_remote_client_signer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/base_signer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/__secfile__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/cass_connector.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/defines.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/keystore.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signature_package.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signature_response.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signing_package.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signing_request_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signing_request_oid.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signing_request_sbl.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signing_request_secure_boot_2.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/signing_request_secure_boot_3.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass/singleton.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/cass_signer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/hsm_signer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/local_v2_signer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/openssl_signer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/qti_remote_signer.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/remote.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/secimage_remote_client.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/secimage_remote_server.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signer_errors.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signer_output.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/attestation_cert.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/attestation_cert_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/attribute_extractor.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/attribute_extractor_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/attributes.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/attributes_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/certconfigparser.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/certconfigparser_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/certificate.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/certificate_base.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/package.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/signerutils/utils.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/utils/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/utils/certkey.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/utils/hmac.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/signer/utils/misc.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/stager/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/stager/base_stager.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/stager/image_paths_stager.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/stager/meta_build_stager.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/stager/meta_build_tmp_stager.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/isc/stager/secimage_base_stager.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/mbngen/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/mbngen/mbngen.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/mbngen/parsegen/__init__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/mbngen/parsegen/__secfile__.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/mbngen/parsegen/auto_gen_xml_config.pyc
	git update-index --assume-unchanged   QcomPkg/Tools/sectools/sectools/features/mbngen/parsegen/defines.pyc
	git update-index --assume-unchanged   BaseTools/Source/C/Makefiles/header.makefile
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/adler32.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/compress.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/configure.log
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/crc32.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/deflate.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/gzclose.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/gzlib.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/gzread.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/gzwrite.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/infback.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/inffast.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/inflate.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/inftrees.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/libz.a
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/minigzip
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/minigzip.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/trees.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/uncompr.o
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/zconf.h
	git update-index --assume-unchanged   BaseTools/Source/C/minigzip/zlib/zutil.o
	git update-index --assume-unchanged   QcomPkg/delcommonts.pyc
