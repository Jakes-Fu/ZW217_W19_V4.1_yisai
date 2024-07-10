@echo off
REM check project name
::*****************************************************************************************
SET project=UWS6121E_WC_1H00_NoGNSS
::*****************************************************************************************

SET root=%cd%\..\..\
SET pack_path=%root%.\build\%project%_builddir
SET img_path=%root%.\build\%project%_builddir\img

::note infomation
echo ============INFO=============
echo [project name]: %project%
echo [image   path]: image path%img_path%
echo ============END==============
::rename stone
SET stone_img=%img_path%\*_stone.img
SET stone_imgbin=%img_path%\*_stone.img.bin
copy %img_path%\*_stone.img %img_path%\*_stone.img.bin

::check stone file
if not exist %img_path%\*_stone.img.bin goto exit

::generate package
call ota\cmiot\pack\cmiot_pack.bat "C:\Program Files\WinRAR\WinRAR.exe" oneos_config.h %img_path%
goto end

:end
move %cd%\*.zip %pack_path%\

:exit
echo generate stone img.bin file failed