@echo off
net session >nul 2>&1
if %errorLevel% neq 0 (
    color 0c
    echo This script must be run as administrator.
    pause
    exit /b 1
)    
git update-index --assume-unchanged Conf/.cache/build.db
git update-index --assume-unchanged QcomPkg/Tools/sectools/sectools/features/isc/parsegen/mbn_tools.pyc
git update-index --assume-unchanged QcomPkg/delcommonts.pyc

set TARGET_DIR=C:\Apps\LLVM\3.9.1

if not exist "%TARGET_DIR%" (
    echo Setting up SnapdragonLLVM 3.9.1
    
    mkdir "%TARGET_DIR%" 2>nul
    
    xcopy /E /I /Y "%CD%\SDLLVM_WIN\*" "%TARGET_DIR%"
    
    echo Setup complete!
) else (
    echo SDLLVM already installed, skipping
)