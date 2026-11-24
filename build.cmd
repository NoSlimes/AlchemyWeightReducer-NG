@echo off
rem Builds a CMake preset with the MSVC environment set up, so it works from any shell.
rem Usage: build.cmd [preset]   (default: x64-debug)
setlocal

set "PRESET=%~1"
if "%PRESET%"=="" set "PRESET=x64-debug"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo Could not find vswhere.exe. Is Visual Studio installed?
    exit /b 1
)

set "VSDIR="
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -property installationPath`) do set "VSDIR=%%i"
if not defined VSDIR (
    echo Could not find a Visual Studio installation.
    exit /b 1
)

if not defined VCPKG_ROOT (
    if exist "%VSDIR%\VC\vcpkg\scripts\buildsystems\vcpkg.cmake" (
        set "VCPKG_ROOT=%VSDIR%\VC\vcpkg"
    )
)

if not defined VCPKG_ROOT (
    echo VCPKG_ROOT is not set and Visual Studio's bundled vcpkg was not found.
    exit /b 1
)

call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 (
    echo Failed to set up the MSVC environment.
    exit /b 1
)

cmake --preset "%PRESET%" || exit /b 1
cmake --build --preset "%PRESET%" || exit /b 1

echo Built with preset "%PRESET%".