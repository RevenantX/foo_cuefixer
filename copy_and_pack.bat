@echo off
rem change directory to bat file path
cd %~dp0
mkdir pack
mkdir pack\arm64
mkdir pack\x64
copy buildarm64\src\Release\foo_cuefixer.dll pack\arm64\
copy build64\src\Release\foo_cuefixer.dll pack\x64\
copy build32\src\Release\foo_cuefixer.dll pack\
cd pack
"C:\Program Files\7-Zip\7z.exe" a -mx9 -tzip ..\foo_cuefixer.fb2k-component *
cd ..
pause