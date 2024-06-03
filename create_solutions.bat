@echo off
rem change to your path
set sdk_path="E:\Projects\foobar2000sdk\SDK-2023-09-23"
cd %~dp0
mkdir build32
mkdir build64
mkdir buildarm64
cd build32
cmake .. -AWin32 -Dfoobar2000sdk=%sdk_path%
cd ..\build64
cmake .. -Ax64 -Dfoobar2000sdk=%sdk_path%
cd ..\buildarm64
cmake .. -AARM64 -Dfoobar2000sdk=%sdk_path%