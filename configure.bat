@echo off
setlocal
REM Configure the plugin. Dependencies come from vcpkg; CommonLib comes from the extern submodule.

call "%~dp0find-msvc.bat"
if errorlevel 1 exit /b 1

cd /d "%~dp0"
cmake --preset release -DBUILD_OFFLINE_TOOLS=OFF
