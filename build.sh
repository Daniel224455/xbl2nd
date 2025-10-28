#!/bin/bash
export BUILDROOT=$(pwd)
export SECTOOLS=$(pwd)/QcomPkg/Tools/sectools/sectools.py
export SECTOOLS_DIR=$(pwd)/QcomPkg/Tools/sectools
export LLVMBIN=$(pwd)/SDLLVM/bin

DEVICE=""
RELEASE=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -d)
            DEVICE="$2"
            shift 2
            ;;
        -r)
            RELEASE="$2"
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

if [ -z "$DEVICE" ]; then
    echo "Error: Device name required. Use -d <device>"
    echo "Usage: ./buildxbl.sh -d <device> -r <release>"
    exit 1
fi

if [ -z "$RELEASE" ]; then
    echo "Error: Release type required. Use -r <release>"
    echo "Usage: ./buildxbl.sh -d <device> -r <release>"
    exit 1
fi

if [ ! -f "Resources/Configs/$DEVICE.conf" ]; then
    echo "Error: Failed to find configuration for $DEVICE, device possibly not ported?"
    exit 1
fi

source Resources/Configs/$DEVICE.conf

echo "Setting python2.7 as default ..."
sudo ln -sf /usr/bin/python2.7 /usr/bin/python

echo "Building BootShim..."
cd BootShim
rm BootShim.elf BootShim.bin
make UEFI_BASE=0x9FC00000 UEFI_SIZE=0x00300000
cd ..

echo "Setting up build environment.."
bash edksetup.sh
python -u QcomPkg/buildex.py --bldconf QcomPkg/SDM845Pkg/Devices/$MFG/$DEVICE/buildconfig.json --variant LA -r $RELEASE -t SDM845Pkg
./Resources/Scripts/$DEVICE.sh