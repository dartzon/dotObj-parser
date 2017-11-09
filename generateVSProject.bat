@echo OFF
if /i {%1}=={ECHO} echo ON&SHIFT

set CMAKELIST_DIR=%cd%

set BUILD_DIR=build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

if exist CMakeCache.txt del CMakeCache.txt

if "%1" == "-test" (cmake -G "Visual Studio 14 2015 Win64" %CMAKELIST_DIR% -DCMAKE_BUILD_TYPE=release -DBUILD_TESTS=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1) else (cmake -G "Visual Studio 14 2015 Win64" %CMAKELIST_DIR% -DCMAKE_BUILD_TYPE=release -DCMAKE_EXPORT_COMPILE_COMMANDS=1)