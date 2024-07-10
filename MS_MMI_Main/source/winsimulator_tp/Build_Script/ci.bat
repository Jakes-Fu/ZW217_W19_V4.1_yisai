@echo off

REM --- checkout files: Call coci.pl ---

cd script
REM ccperl coci.pl ci 2>c:\coci_log.txt
ccperl ci_lib.pl  2>>c:\coci_log.txt
cd ..

REM notepad c:\coci_log.txt