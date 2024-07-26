@echo off

for /f "usebackq delims=" %%j in (`dir /s /b *.uvprojx`) do (
if exist %%j (
set UV_PRO_PATH="%%j"))

set curdir=%~dp0
set UV=C:\Keil_v5\UV4\UV4.exe
::set UV_PRO_PATH=%curdir%\meter.uvprojx
echo Init building ...
echo >build_log

::r rebuid
::b Builds the last current
::c clean project
::cr clean and rebuid
::d start debug
echo %UV% -j16 -b %UV_PRO_PATH% -o %cd%\build_log
%UV% -j16 -b %UV_PRO_PATH% -o %cd%\build_log
type build_log
echo Done.
pause