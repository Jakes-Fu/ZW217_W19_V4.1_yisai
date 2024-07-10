@echo off

set cms_id=0
set block_size=0
set info_file=info.txt

call:findstr1 CMS_ID_SET_PID %def_path% cms_id

echo #define CMIOT_FOTA_SERVICE_PRODUCT_ID "%cms_id%" > %info_file%
call:findstr2 CMIOT_FOTA_CONTRAST_BLOCK_SIZE %def_path% block_size
echo #define CMIOT_FOTA_CONTRAST_BLOCK_SIZE "%block_size%" >> %info_file%

if "%algorithm%"=="0" (
    echo #define CMIOT_FOTA_SDK_VER "%lusun_ver%" >> %info_file%
) else if "%algorithm%"=="3" (
    echo #define CMIOT_FOTA_SDK_VER "%luckspar_ver%" >> %info_file%
) else if "%algorithm%"=="4" (
    echo "error, only luckpand only support bin dir."
    exit /B -1
) else (
    echo #define CMIOT_FOTA_SDK_VER "%wosun_ver%" >> %info_file%
)

if "%algorithm%"=="1" (set /A wosun=0) else (set /A wosun=1)
if "%algorithm%"=="0" (echo lusun) else if "%algorithm%"=="3" (
    echo #define CMIOT_FOTA_AVAILABLE_RAM_SIZE "%ram_size%" >> %info_file%
    echo #define CMIOT_FOTA_DOWNLOAD_PART_SIZE "%dl_size%" >> %info_file%
) else (
    echo #define CMIOT_FOTA_AVAILABLE_RAM_SIZE "%ram_size%" >> %info_file%
    echo #define CMIOT_FOTA_FILENAME1 "%cmiot%-D" >> %info_file%
    echo #define CMIOT_FOTA_ADDRESS1   "%appaddr%" >> %info_file%
    echo #define CMIOT_FOTA_REGION_LEN   "%size%" >> %info_file%
    echo #define CMIOT_FOTA_BLOCK_LEN "%block%" >> %info_file%
    echo #define CMIOT_FOTA_PATCH_FORMAT   "%wosun%" >> %info_file%
)

echo #define CMIOT_BUILD_TIME   "%d%_%t%" >> %info_file%
echo #define CMIOT_APP_VERSION "%version%" >> %info_file%

exit /B 0

:findstr1
    for /f tokens^=^2^ delims^=^"^= %%i in ('findstr "%1" %2') do set "%3=%%i"
GOTO:EOF

:findstr2
    for /f "tokens=2,3" %%i in ('findstr "%1" %2') do if %%i==%1 set /A "%3=%%j"
GOTO:EOF