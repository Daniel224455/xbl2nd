@echo off
set PATH=C:\python27;%PATH%
set DEVICE=
set RELEASE=

:parse
if "%1"=="-d" set DEVICE=%2& shift& shift& goto parse
if "%1"=="-r" set RELEASE=%2& shift& shift& goto parse
if not "%1"=="" shift& goto parse

if "%DEVICE%"=="" (
    echo Error: Device name required. Use -d ^<device^>
    echo Usage: buildxbl -d ^<device^> -r ^<release^>
    pause
    exit /b 1
)

if "%RELEASE%"=="" (
    echo Error: Release type required. Use -r ^<release^>
    echo Usage: buildxbl -d ^<device^> -r ^<release^>
    pause
    exit /b 1
)

call shimbuild.bat
call Resources\Configs\%DEVICE%.bat
if errorlevel 1 (
    echo Error: Failed to load configuration for %DEVICE%, device possibly not ported?
    pause
    exit /b 1
)
python -u QcomPkg/buildex.py --bldconf QcomPkg/SDM845Pkg/Devices/%MFG%/%DEVICE%/buildconfig.json --variant LA -r %RELEASE% -t SDM845Pkg
call Resources\Scripts\%DEVICE%.bat