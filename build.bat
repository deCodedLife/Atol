echo off
cls

set BUILD_TYPE="release"
set PROJECT_NAME="Atol-server2"
set BUILD_DIR="D:\Workflow\build-%PROJECT_NAME%-Qt_6_Static-Release"
set PROJECT_DIR="D:\Workflow\Atol-server3"
set QMAKE_EXE="E:\Qt_static\bin\qmake6.exe"
set MSVC_BUILD="C:\Users\DRago\msvc_make.bat"
set VCVARS32="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"

cd %BUILD_DIR%

echo ------------------------------------------------------------
echo Building project...

cd %BUILD_DIR%
%QMAKE_EXE% D:\Workflow\Atol-server3\Atol-server2.pro -spec win32-msvc "CONFIG-=qml_debug" "CONFIG+=qtquickcompiler"

echo ------------------------------------------------------------
echo Compiling...

cd %BUILD_DIR%
call %VCVARS32%
call %MSVC_BUILD% -j8

echo ------------------------------------------------------------
upx "%BUILD_DIR%/%BUILD_TYPE%/%PROJECT_NAME%.exe"
echo ------------------------------------------------------------

echo DONE
cd %PROJECT_DIR%
pause