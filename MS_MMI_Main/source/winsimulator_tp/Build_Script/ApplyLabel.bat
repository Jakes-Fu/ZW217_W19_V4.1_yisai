@echo off

REM --- apply label : Call mklabel.pl ---

cd script
ccperl mklabel.pl MSDEV_R2.8.0003.BL0003 2>c:\mklabel_log.txt
cd ..

notepad c:\mklabel_log.txt