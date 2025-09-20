#!/bin/bash
export BUILDROOT=$(pwd)
export SECTOOLS=$(pwd)/QcomPkg/Tools/sectools/sectools.py
export SECTOOLS_DIR=$(pwd)/QcomPkg/Tools/sectools
export LLVMBIN=$(pwd)/SDLLVM/bin

echo "Setting python2.7 as default ..."
sudo ln -sf /usr/bin/python2.7 /usr/bin/python

#echo "Building BootShim..."
#cd BootShim
#rm BootShim.elf BootShim.bin
#make UEFI_BASE=0x9FC00000 UEFI_SIZE=0x00300000
#cd ..

echo "Setting up build environment.."
bash edksetup.sh
python -u QcomPkg/buildex.py --variant LA -r DEBUG -t SDM845Pkg
# for now, device specific
#./resources/scripts/enchilada.sh