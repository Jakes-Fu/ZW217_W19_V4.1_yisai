@echo off

set SRC=build\%1_builddir

if exist build\wre_sdk_from_mocor goto kernelfinished

mkdir build\wre_sdk_from_mocor\bin\skin
xcopy MS_MMI\source\winsimulator_tp\skin\*.*             build\wre_sdk_from_mocor\bin\skin\             /y /e /h /q 
xcopy MS_MMI\source\winsimulator_tp\win_platform\SIM_Set_Files\*.*    build\wre_sdk_from_mocor\bin\SIM_Set_Files\    /y /e /h /q 
mkdir build\wre_sdk_from_mocor\VcAddins
mkdir build\wre_sdk_from_mocor\VcTemplate
copy  wre\sdk\tools\FileTool\bin\WRE-Addin1.0.dll        build\wre_sdk_from_mocor\VcAddins\WRE-Addin1.0.dll /y
copy  wre\sdk\tools\FileTool\bin\WRE-Addin1.0.dll        build\wre_sdk_from_mocor\bin\WRE-Addin1.0.dll /y
copy  "wre\sdk\tools\WRE App\bin\WRE App.awx"           "build\wre_sdk_from_mocor\VcTemplate\WRE App.awx"  /y
xcopy tools\DEBUG_TOOL\CHANNELSERVER\Bin\*.*             build\wre_sdk_from_mocor\tool\CHANNELSERVER\      /y /e /h /q
xcopy tools\DEBUG_TOOL\LOGEL\Bin\*.*                     build\wre_sdk_from_mocor\tool\LOGEL\              /y /e /h /q
xcopy tools\DEBUG_TOOL\ResearchDownload\Bin\*.*          build\wre_sdk_from_mocor\tool\ResearchDownload\   /y /e /h /q
xcopy tools\DEBUG_TOOL\SCIUSB2SERIAL\x86\*.*             build\wre_sdk_from_mocor\tool\SCIUSB2SERIAL\      /y /e /h /q
xcopy wre\sdk\interface\arm\*.*                          build\wre_sdk_from_mocor\lib\arm\                 /y /e /h /q
xcopy wre\sdk\interface\win32\*.*                        build\wre_sdk_from_mocor\lib\win32\               /y /e /h /q
xcopy MS_MMI\source\winsimulator_tp\win_platform\bin\disk\*.img build\wre_sdk_from_mocor\bin\disk\             /y /e /h /q
xcopy MS_MMI\source\winsimulator_tp\win_platform\bin\*.* build\wre_sdk_from_mocor\bin\   /y /q /h
copy  wre\sdk\tools\wrebuild\install\szip.dll            build\wre_sdk_from_mocor\bin\szip.dll   /y
copy  wre\sdk\tools\wrebuild\install\wrebuild.exe        build\wre_sdk_from_mocor\bin\wrebuild.exe   /y
copy  wre\sdk\tools\wrepack\bin\wrepack.exe              build\wre_sdk_from_mocor\bin\wrepack.exe   /y
set MAKEFLAGS=
cd wre\wrekernel
cd hw_prj
call buildwrekernel.bat
cd ..

cd pc_proj
del /f /q error.txt
del /f /q result.txt
cmd /c nmake wrekernel.mak 1>error.txt 2>result.txt
cd ..

cd ..\..\
mkdir build\wre_sdk_from_mocor\sdcard\wre
copy wre\wrekernel\hw_prj\wrekernel.wrd                           build\wre_sdk_from_mocor\sdcard\wre\wrekernel.wrd     /y
mkdir build\wre_sdk_from_mocor\prjfile
mkdir build\wre_sdk_from_mocor\bin
copy  wre\wrekernel\windowsexport\Debug\windowsexport.dll         build\wre_sdk_from_mocor\bin\windowsexport.dll     /y
copy  wre\wrekernel\pc_proj\Debug\wrekernel.dll                   build\wre_sdk_from_mocor\bin\wrekernel.dll     /y
xcopy wre\wrekernel\include\sdk\*.*                               build\wre_sdk_from_mocor\include\ /y /e /h /q

:kernelfinished

mkdir build\wre_sdk_from_mocor\prjfile\%1
copy %SRC%\img\mmi_res_*_win.bin     build\wre_sdk_from_mocor\prjfile\%1\ /y
mkdir build\wreinstall\pac\%1
copy %SRC%\img\*.pac build\wreinstall\pac\%1   /y
copy %SRC%\img\*.map build\wreinstall\pac\%1   /y


