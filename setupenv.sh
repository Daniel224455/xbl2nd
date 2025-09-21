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
  
if [ ! -d "/pkg/qct/software/llvm/release/arm/3.9.1" ]; then
    echo "Setting up SnapdragonLLVM 3.9.1"
    sudo mkdir /pkg
    sudo mkdir -p /pkg/qct/software/llvm/release/arm/3.9.1
    sudo cp -r $(pwd)/SDLLVM/* /pkg/qct/software/llvm/release/arm/3.9.1
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

git update-index --assume-unchanged QcomPkg/delcommonts.pyc
git update-index --assume-unchanged BaseTools/Source/C/minigzip/zlib/configure.log
git update-index --assume-unchanged BaseTools/Source/C/minigzip/zlib/libz.a
git update-index --assume-unchanged BaseTools/Source/C/minigzip/zlib/minigzip
git update-index --assume-unchanged BaseTools/Source/C/minigzip/zlib/zconf.h
git update-index --assume-unchanged QcomPkg/Tools/sectools/sectools/features/isc/parsegen/mbn_tools.pyc