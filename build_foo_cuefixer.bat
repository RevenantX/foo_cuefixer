@echo off
setlocal
for %%I in ("%~dp0.") do set "repo=%%~fI"
pushd "%repo%"

set "sdk_path=D:\Projects\foobar2000sdk\SDK-2026-10-01"
set "cmake="
set "msbuild="
set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

for /f "delims=" %%I in ('where cmake.exe 2^>nul') do set "cmake=%%I"
if not defined cmake (
    echo ERROR: CMake was not found on PATH.
    set "result=1"
    goto :finish
)

for /f "delims=" %%I in ('where msbuild.exe 2^>nul') do if not defined msbuild set "msbuild=%%I"
if not defined msbuild if exist "%vswhere%" (
    for /f "usebackq delims=" %%I in (`"%vswhere%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\Current\Bin\MSBuild.exe`) do if not defined msbuild set "msbuild=%%I"
)
if not defined msbuild (
    echo ERROR: MSBuild was not found. Install Visual C++ Build Tools.
    set "result=1"
    goto :finish
)

call "%repo%\build_sdk.bat"
if errorlevel 1 goto :failed

call :configure build32 Win32
if errorlevel 1 goto :failed
call :build build32 Win32
if errorlevel 1 goto :failed

call :configure build64 x64
if errorlevel 1 goto :failed
call :build build64 x64
if errorlevel 1 goto :failed

call :configure buildarm64ec ARM64EC
if errorlevel 1 goto :failed
call :build buildarm64ec ARM64EC
if errorlevel 1 goto :failed

echo Release builds completed for x86, x64, and ARM64EC.
set "result=0"
goto :finish

:configure
echo Configuring %~2...
"%cmake%" --fresh -S "%repo%" -B "%repo%\%~1" -G "Visual Studio 18 2026" -A %~2 "-Dfoobar2000sdk=%sdk_path%"
exit /b %errorlevel%

:build
echo Building Release %~2...
set "solution=%repo%\%~1\foo_cuefixer.slnx"
if not exist "%solution%" set "solution=%repo%\%~1\foo_cuefixer.sln"
if not exist "%solution%" (
    echo ERROR: No generated solution found in "%repo%\%~1"
    exit /b 1
)
"%msbuild%" "%solution%" /m /nodeReuse:false /p:Configuration=Release /p:Platform=%~2 /verbosity:minimal
exit /b %errorlevel%

:failed
set "result=%errorlevel%"

:finish
popd
exit /b %result%
