@echo off
echo "starting building ......"
@echo %1%
SET CURRENTPATH=%CD%
@echo %CURRENTPATH%

set path=%path%;C:\Windows\System32
set path=%path%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\
SET CURRENTPATH=%CD%
@echo %CURRENTPATH%
devenv build\%1%_builddir\win\MSDev2010.sln /rebuild

echo this build finish
