# XBL2nd packing mechanism

# Build an Android kernel that is actually UEFI disguised as the Kernel.
cat ./BootShim/BootShim.bin "./Build/SDM845LA_Core/DEBUG_CLANG39LINUX/FV/SDM845_EFI.fd" > "./Build/SDM845LA_Core/DEBUG_CLANG39LINUX/FV/SDM845_EFI.fd-bootshim"||exit 1
gzip -c "./Build/SDM845LA_Core/DEBUG_CLANG39LINUX/FV/SDM845_EFI.fd-bootshim" > "./Build/SDM845LA_Core/DEBUG_CLANG39LINUX/FV/SDM845_EFI.fd-bootshim.gz"||exit 1
# This will be our second-stage payload which can be loaded from ABL.
cat "./Build/SDM845LA_Core/DEBUG_CLANG39LINUX/FV/SDM845_EFI.fd-bootshim.gz" ./Resources/Blobs/enchilada.dtb > ./Resources/payload.bin||exit 1

# Create a bootable Android boot image.
python3 ./Resources/Scripts/mkbootimg.py \
  --kernel ./Resources/payload.bin \
  --ramdisk ./Resources/ramdisk \
  --kernel_offset 0x00000000 \
  --ramdisk_offset 0x00000000 \
  --tags_offset 0x00000000 \
  --os_version 13.0.0 \
  --os_patch_level "$(date '+%Y-%m')" \
  --header_version 1 \
  -o xbl2nd-enchilada.img \
  ||_error "\nFailed to create Android Boot Image!\n"