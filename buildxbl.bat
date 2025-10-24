@echo off
set PATH=C:\python27;%PATH%
call shimbuild.bat
python -u QcomPkg/buildex.py --variant LA -r DEBUG -t SDM845Pkg
call Resources/Scripts/enchilada.bat