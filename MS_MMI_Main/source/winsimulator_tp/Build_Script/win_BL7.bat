@echo off
echo  --- building on Windows ---

REM NMAKE /f "..\..\Tool\MSDev\MSDev.mak" /NOLOGO CFG="MSDev - Win32 Debug"

call co.bat

msdev "..\Source\MSDev_Mod.dsw" /MAKE "MSDev - Win32 Debug" /REBUILD /OUT build.log

call ci.bat

rem 检查是否生成exe文件
rem IF EXIST ..\..\Tool\MSDev\MSDev.exe (ECHO Build successful! ) ELSE (ECHO Build failed! )

REM cd script
REM ccperl analyse_log.pl "MSDev.dsw Project have finished buiding!"  VC
REM cd ..

REM pause