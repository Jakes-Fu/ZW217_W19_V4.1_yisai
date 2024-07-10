@echo off 
setlocal EnableDelayedExpansion

if x%1==x (
    echo no input parameters were found
    echo you can input %0 --help for detail
    GOTO:EOF
)
if x%1==x--help (
    call :usage %0
    GOTO:EOF
)

if x%3==x (
    echo "Input error."
    call :usage %0
    GOTO:EOF
)

set is_dir=0
dir /ad %3 1>nul 2>nul&&set is_dir=1
if x%4==x (
    if "%is_dir%"=="0" (
        call %~dp0\cmiot_bin.bat %1 %2 %3
    ) else (
        call %~dp0\cmiot_multi_bin.bat %1 %2 %3
    )
) else (
    call %~dp0\cmiot_axf.bat %1 %2 %3 %4
)

GOTO :EOF

:usage
    echo usage1: %1 fromelf.exe WinRAR.exe^|7z.exe oneos_config.h axf_file, eg: %1 D:\tools\fromelf.exe D:\tools\WinRAR.exe oneos_config.h Obj\oneos.axf
    echo usage2: %1 WinRAR.exe^|7z.exe oneos_config.h bin_file, eg: %1 D:\tools\WinRAR.exe oneos_config.h out\oneos.bin
    echo usage3: %1 WinRAR.exe^|7z.exe oneos_config.h bin_dir, eg %1 D:\tools\WinRAR.exe oneos_config.h multi_bin\
GOTO :EOF