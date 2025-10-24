@echo off
rem XBL2nd packing mechanism

rem Build an Android kernel that is actually UEFI disguised as the Kernel.
copy /b BootShim\BootShim.bin + Build\SDM845LA_Core\DEBUG_CLANG39WIN\FV\SDM845_EFI.fd Build\SDM845LA_Core\DEBUG_CLANG39WIN\FV\SDM845_EFI.fd-bootshim
call Resources\Scripts\gzip.exe -c "Build\SDM845LA_Core\DEBUG_CLANG39WIN\FV\SDM845_EFI.fd-bootshim" > "Build\SDM845LA_Core\DEBUG_CLANG39WIN\FV\SDM845_EFI.fd-bootshim.gz"
copy /b "Build\SDM845LA_Core\DEBUG_CLANG39WIN\FV\SDM845_EFI.fd-bootshim.gz" + "Resources\Blobs\enchilada.dtb" "Resources\payload.bin"

call python3 ./Resources/Scripts/mkbootimg.py ^
  --kernel ./Resources/payload.bin ^
  --ramdisk ./Resources/ramdisk ^
  --kernel_offset 0x00000000 ^
  --ramdisk_offset 0x00000000 ^
  --tags_offset 0x00000000 ^
  --os_version 13.0.0 ^
  --os_patch_level 2025-10 ^
  --header_version 1 ^
  -o xbl2nd-enchilada.img ^