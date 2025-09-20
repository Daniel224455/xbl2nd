@echo off



:: Python Interpreter Paths

set python_path_default="C:\Python34"

set python_path_dart="C:\Apps\Python34"

set python_path_na_domain="\\qctdfsrt\prj\vlsi\vlsi_verify\vi_tools\sve_tools\Tools_prj\ddr\Apps\Python34"

set python_path_ap_domain="\\qctdfsrt\blr\prj\vlsi\vlsi_verify\vi_tools\sve_tools\Tools_prj\ddr\Apps\Python34_dcb"



:: Required Python Packages (Lib\site-packages)

set path_xlrd="Lib\site-packages\xlrd"



:: Look for a Python interpreter with the required packages

call :find_python_interpreter

if not defined python_path (

    echo ERROR: No Python interpreter with xlrd package found!

    goto end

)



:: Look for dsf.py

if not exist dsf.py goto cannot_find_dsf.py

echo Loading Python interpreter from %python_path%...

%python_path%\python.exe dsf.py



:end

set /p DUMMY=Press ENTER to close window...

exit /b



:cannot_find_dsf.py

echo Unable to locate dsf.py

goto end



:: Function to check python interpreter paths

:find_python_interpreter

    call :check_python_packages %python_path_default%

    if defined python_path exit /b

    

    call :check_python_packages %python_path_dart%

    if defined python_path exit /b

    

    if "%USERDOMAIN%"=="AP" call :check_python_packages %python_path_ap_domain%

    if defined python_path exit /b

    

    if not x%COMPUTERNAME:BDC=%==x%COMPUTERNAME% call :check_python_packages %python_path_ap_domain%

    if defined python_path exit /b

    

    if not x%COMPUTERNAME:blr=%==x%COMPUTERNAME% call :check_python_packages %python_path_ap_domain%

    if defined python_path exit /b

    

    call :check_python_packages %python_path_na_domain%

    

    exit /b



:: Function to check required python packages

:check_python_packages

    if not exist %1\%path_xlrd% (

        exit /b

    )

    set python_path=%1

    exit /b