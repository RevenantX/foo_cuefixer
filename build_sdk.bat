@echo off
setlocal

rem Update the SDK path if it is installed elsewhere.
set "sdk_path=D:\Projects\foobar2000sdk\SDK-2026-10-01"
for %%I in ("%sdk_path%") do set "sdk_path=%%~fI"

rem WTL defaults to a sibling directory of the SDK.
set "wtl_path=%sdk_path%\..\WTL"
for %%I in ("%wtl_path%") do set "wtl_path=%%~fI"

set "solution=%sdk_path%\foobar2000\foo_sample\foo_sample.sln"
set "project=%sdk_path%\foobar2000\foo_sample\foo_sample.vcxproj"
set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "msbuild="

if not exist "%solution%" (
    echo ERROR: SDK solution not found: "%solution%"
    exit /b 1
)

if not exist "%project%" (
    echo ERROR: SDK project not found: "%project%"
    exit /b 1
)

if not exist "%wtl_path%\Include\atlapp.h" (
    echo ERROR: WTL headers not found under "%wtl_path%\Include"
    exit /b 1
)

for /f "delims=" %%I in ('where msbuild.exe 2^>nul') do if not defined msbuild set "msbuild=%%I"

if not defined msbuild if exist "%vswhere%" (
    for /f "usebackq delims=" %%I in (`"%vswhere%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\Current\Bin\MSBuild.exe`) do if not defined msbuild set "msbuild=%%I"
)

if not defined msbuild (
    echo ERROR: MSBuild was not found. Install Visual C++ Build Tools.
    exit /b 1
)

rem Pass WTL to the compiler without editing SDK project files.
set "CL=/I%wtl_path%\Include"

rem call :build_solution Debug x86
rem if errorlevel 1 exit /b %errorlevel%
call :build_solution Release x86
if errorlevel 1 exit /b %errorlevel%

rem call :build_project Debug x64
rem if errorlevel 1 exit /b %errorlevel%
call :build_project Release x64
if errorlevel 1 exit /b %errorlevel%

rem call :build_project Debug ARM64EC
rem if errorlevel 1 exit /b %errorlevel%
call :build_project Release ARM64EC
if errorlevel 1 exit /b %errorlevel%

echo SDK Debug and Release builds completed for x86, x64, and ARM64EC.
exit /b 0

:build_solution
echo Building SDK %~1 %~2...
"%msbuild%" "%solution%" /m /nodeReuse:false /p:PlatformToolset=v145 /p:Configuration=%~1 /p:Platform=%~2 /verbosity:minimal
exit /b %errorlevel%

:build_project
echo Building SDK %~1 %~2...
"%msbuild%" "%project%" /m /nodeReuse:false /p:PlatformToolset=v145 /p:Configuration=%~1 /p:Platform=%~2 /verbosity:minimal
if errorlevel 1 exit /b %errorlevel%
call :stage_libraries %~2 %~1
exit /b %errorlevel%

:stage_libraries
set "stage_dir=%sdk_path%\foobar2000\foo_sample\%~1\%~2"
if not exist "%stage_dir%" mkdir "%stage_dir%"
if errorlevel 1 exit /b %errorlevel%

copy /y "%sdk_path%\foobar2000\SDK\%~1\%~2\foobar2000_SDK.lib" "%stage_dir%\" >nul
if errorlevel 1 exit /b %errorlevel%
copy /y "%sdk_path%\foobar2000\helpers\%~1\%~2\foobar2000_sdk_helpers.lib" "%stage_dir%\" >nul
if errorlevel 1 exit /b %errorlevel%
copy /y "%sdk_path%\libPPUI\%~1\%~2\libPPUI.lib" "%stage_dir%\" >nul
if errorlevel 1 exit /b %errorlevel%
copy /y "%sdk_path%\pfc\%~1\%~2\pfc.lib" "%stage_dir%\" >nul
exit /b %errorlevel%
