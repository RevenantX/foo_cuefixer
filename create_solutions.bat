@echo off
rem change to your path
set sdk_path="D:\Projects\foobar2000sdk\SDK-2026-10-01"
cd %~dp0
mkdir build32
mkdir build64
mkdir buildarm64ec
cd build32
cmake .. -AWin32 -Dfoobar2000sdk=%sdk_path%
cd ..\build64
cmake .. -Ax64 -Dfoobar2000sdk=%sdk_path%
cd ..\buildarm64ec
cmake .. -AARM64EC -Dfoobar2000sdk=%sdk_path%