echo off
cls

set PROJECT_NAME="Atol-server2"
set BUILD_DIR="D:\Workflow\build-"%PROJECT_NAME%"-Qt_6_Static-Release"
set PROJECT_DIR="D:\Workflow\Atol-server3"
set MSVC_BUILD="C:\Users\DRago\msvc_make.bat"
set VCVARS32="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"

cd %BUILD_DIR%

echo ------------------------------------------------------------
echo Clean project...

cd %BUILD_DIR%
call %VCVARS32%
call %MSVC_BUILD% -j8 clean

echo DONE
cd %PROJECT_DIR%