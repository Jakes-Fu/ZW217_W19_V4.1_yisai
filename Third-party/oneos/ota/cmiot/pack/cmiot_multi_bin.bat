@echo off 
setlocal EnableDelayedExpansion

set zip_patch=%1
set def_path=%2
set dir_path=%3
set version=version
set algorithm=0
set appaddr=0
set block_size=0
set wosun=0
set size=0
set lusun_ver=0
set wosun_ver=0
set luckspar_ver=0
set luckpand_ver=0
set app_ver=0
set ram_size=0
set dl_size=0
set cmiot=
set info_file=info.txt

for /f "tokens=2 delims==" %%a in ('wmic path win32_operatingsystem get LocalDateTime /value') do (
    set datetime=%%a
)
set d=%datetime:~0,4%%datetime:~4,2%%datetime:~6,2%
set h=%time:~0,2%
set h=%h: =0%
set t=%h%%time:~3,2%%time:~6,2%

call:findstr1 CMIOT_FIRMWARE_VERSION %def_path% version
call:findstr1 CMIOT_FOTA_LUSUN_VERSION %def_path% lusun_ver
call:findstr1 CMIOT_FOTA_WOSUN_VERSION %def_path% wosun_ver
call:findstr1 CMIOT_FOTA_LUCKSPAR_VERSION %def_path% luckspar_ver
call:findstr1 CMIOT_FOTA_LUCKPAND_VERSION %def_path% luckpand_ver
call:findstr2 CMIOT_FOTA_ALGORITHM %def_path% algorithm
call:findstr2 CMIOT_FLASH_APP_ADDR %def_path% appaddr
call:findstr2 CMIOT_DEFAULT_SECTOR_SIZE %def_path% block
call:findstr2 CMIOT_UPDATE_SIZE %def_path% size
call:findstr2 CMIOT_FOTA_AVAILABLE_RAM_SIZE %def_path% ram_size

set /A size=size/1024
set /A dl_size=size
set /A ram_size=ram_size/1024

echo algorithm is %algorithm%

if not "%algorithm%"=="1" (
    echo "error, only luckpand support multi bin."
    GOTO:EOF
)

findstr "CMIOT_USING_CMS" %def_path% > nul
if %errorlevel% neq 0 (
    echo "error, only cms support luckpand."
    GOTO:EOF
)

call:findstr1 CMS_ID_SET_PID %def_path% cms_id

echo #define CMIOT_FOTA_SERVICE_PRODUCT_ID "%cms_id%" > %info_file%
call:findstr2 CMIOT_FOTA_CONTRAST_BLOCK_SIZE %def_path% block_size
echo #define CMIOT_FOTA_CONTRAST_BLOCK_SIZE "%block_size%" >> %info_file%
echo #define CMIOT_FOTA_SDK_VER "%luckpand_ver%" >> %info_file%
echo #define CMIOT_FOTA_AVAILABLE_RAM_SIZE "%ram_size%" >> %info_file%
echo #define CMIOT_FOTA_DOWNLOAD_PART_SIZE "%dl_size%" >> %info_file%

for /f tokens^=^1^,^2^,^3^ delims^=^"^  %%i in ('findstr  "CMIOT_FOTA_FILENAME" %def_path%') do (
    set file_mcr=%%j
    set idx=!file_mcr:CMIOT_FOTA_FILENAME=!
    findstr "CMIOT_FOTA_USE_FULL!idx!" %def_path% > nul
    if !errorlevel! neq 0 (
            echo %%i %%j "%%k-D" >> %info_file%
    ) else (
        echo %%i %%j "%%k" >> %info_file%
    )
)

for /f tokens^=^1^,^2^,^3^ delims^=^"^  %%i in ('findstr  "CMIOT_FOTA_ADDRESS" %def_path%') do (
    echo %%i %%j "%%k" >> %info_file%
)

echo #define CMIOT_BUILD_TIME   "%d%_%t%" >> %info_file%
echo #define CMIOT_APP_VERSION "%version%" >> %info_file%

for /f tokens^=^2^ delims^=^" %%i in ('findstr "CMIOT_FOTA_FILENAME" %def_path%')  do (
    set cmiot=!cmiot! %dir_path%\%%~i
)

echo+%zip_patch%|findstr "7z.exe" 
if %errorlevel% equ 0 (
    %zip_patch% a -tzip %version%_%d%_%t%.zip %info_file% %cmiot%
) else (
    %zip_patch% a -ep1 -o+ -inul  -iback %version%_%d%_%t%.zip %info_file% %cmiot%
)

del %info_file% /s
echo complete
GOTO:EOF

:findstr1
    for /f tokens^=^2^ delims^=^"^= %%i in ('findstr "%1" %2') do set "%3=%%i"
GOTO:EOF

:findstr2
    for /f "tokens=2,3" %%i in ('findstr "%1" %2') do if %%i==%1 set /A "%3=%%j"
GOTO:EOF