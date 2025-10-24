@echo off
set TARGET=aarch64-linux-gnu
set UEFI_BASE=0x9FC00000
set UEFI_SIZE=0x00300000
set PATH=C:\Apps\LLVM\3.9.1\bin;%PATH%

clang --target=%TARGET% -c BootShim/BootShim.S -o BootShim/BootShim.elf ^
    -DUEFI_BASE=%UEFI_BASE% -DUEFI_SIZE=%UEFI_SIZE%

if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    exit /b 1
)

llvm-objcopy -O binary BootShim/BootShim.elf BootShim/BootShim.bin

if %ERRORLEVEL% NEQ 0 (
    echo Binary conversion failed!
    exit /b 1
)

echo BootShim built successfully