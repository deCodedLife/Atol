echo off
cls

set BUILD_TYPE="release"
set PROJECT_NAME="Atol-server2"
set BUILD_DIR="D:\Workflow\build-Atol-server2-Desktop_Qt_6_3_0_MinGW_64_bit-Debug"
set PROJECT_DIR="D:\Workflow\Atol-server3"
set QMAKE_EXE="E:\Qt_static\bin\qmake6.exe"
set MINGW_BUILD="E:\Qt_bin\Tools\mingw1120_64\bin\mingw32-make.exe"
set VCVARS32="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"

cd %BUILD_DIR%

echo ------------------------------------------------------------
echo Building project...

cd %BUILD_DIR%
%QMAKE_EXE% D:\Workflow\Atol-server3\Atol-server2.pro -spec win32-msvc "CONFIG-=qml_debug" "CONFIG+=qtquickcompiler"

echo ------------------------------------------------------------
echo Compiling...

cd %BUILD_DIR%
%MINGW_BUILD% -j4

echo ------------------------------------------------------------
cd "E:\Qt_bin\6.3.0\mingw_64\bin"
call "D:\Workflow\build-Atol-server2-Desktop_Qt_6_3_0_MinGW_64_bit-Debug\debug\Atol-server2.exe"
echo ------------------------------------------------------------

echo DONE
cd %PROJECT_DIR%
pause