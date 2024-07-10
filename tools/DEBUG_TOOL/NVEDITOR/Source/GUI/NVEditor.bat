echo off

set COMPCP=..\..\..\common\bin\compcp.bat
set SRC_DIR=..\..\..\common\bin
set DST_DIR=..\..\..\Release\NVEditor\Bin
set FILES=Channel.dll DiagChan.dll BMPlatform.dll functions.ini NVEditor.exe NVEditor.ini BMError.ini BMFileType.ini BMTimeout.ini BMError.ini

for %%f in (%FILES%) do (
@call "%COMPCP%" "%SRC_DIR%\%%f" "%DST_DIR%\%%f"
)

