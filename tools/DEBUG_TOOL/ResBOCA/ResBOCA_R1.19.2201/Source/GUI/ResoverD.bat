echo off

set COMPCP=..\..\..\..\common\bin\compcp.bat
set SRC_DIR=..\..\..\..\Release\ResourceEditor\bin
set DST_DIR=..\..\..\..\Release\Resover\Bin
set FILES=SciDllProducer.dll OsdObjBaseD.dll ImgOptmD.dll X_IMGD.dll

set _COMP=ture

for %%f in (%FILES%) do (
@call "%COMPCP%" "%SRC_DIR%\%%f" "%DST_DIR%\%%f" "%_COMP%"
)

