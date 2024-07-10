@echo off
setlocal

set __lintaloa_path=c:\pclint

if "%1" == "" goto HELP_ME

set __project_dir=%1
set __dir=%3
set __files=%2


if not exist %__project_dir%\%__dir% md %__project_dir%\%__dir%


:LINT_ALOA
echo Linting project
%__lintaloa_path%\lint-nt.exe -b -i"c:\pclint\makelint" std.lnt %__project_dir%\%__files% > %__project_dir%\%__dir%\aloa.xml

echo Executing ALOA
%__lintaloa_path%\aloa.exe %__project_dir%\%__dir%\aloa.xml >%__project_dir%\%__dir%\result.txt
type %__project_dir%\%__dir%\result.txt

goto END

:HELP_ME
echo.
echo. makelint <directory> <file.lnt> <result dir>
echo.

:END
endlocal

