::@echo off

for /f "usebackq delims=" %%j in (`dir /s /b *.uvprojx`) do (
if exist %%j (
set UV_PRO_PATH="%%j"))

set UV=C:\Keil_v5\UV4\UV4.exe

%UV% -j0 -f %UV_PRO_PATH% -o Prg_Output

type Prg_Output
echo Done.
pause