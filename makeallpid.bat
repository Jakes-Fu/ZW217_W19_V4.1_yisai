:: 使用方式 mallalllpid.bat [工程名] [是否编译资源]
:: 例子     makeallpid.bat UIS8910FF_mpphone_evt_dualsim N
:: Version : 0.0.2 
:: Update : line 19 --> add new make to decrease the size of PAC 
@echo off
set input_project=%1%
set input_resource=%2%
set single_or_dual=ms.pid

if "%input_project%"=="-h" (
	echo "makeallpid.bat [project_name] [Y/N]-(build resource_main or not)"
	echo "makeallpid.bat UIS8910FF_mpphone_evt_dualsim N"
	GOTO:EOF
)

echo "starting building ......"

make\make_cmd\make -r -R p=%input_project% job=4 -j2 PID=AA new >err_qa
make\make_cmd\make -r -R p=%input_project% job=4 -j2 PID=AA new >err_qa2

:: 单双卡获取 
for  /f "tokens=1-3 delims= " %%i in (project_%1%.mk) do (
	if  "%%i"=="MMI_MULTI_SIM_SYS" (
		echo [D/S] %%k >>buildallpid.txt
		echo [Project] %input_project% >>buildallpid.txt
		echo [Build resource_main] %input_resource%  >>buildallpid.txt
		if "%%k"=="SINGLE" (			
			set single_or_dual=ms_ss.pid
		)
	)
)

:: 从ms.pid/ms_ss.pid中获取LP（AA，QA ...） 然后获取PC（product code）
for  /f "tokens=1-6,* delims=_ " %%a in (%single_or_dual%) do (

	if "%%a"=="PID" (
		if "%%b"=="PC" (					
			if "%%d"=="VALID" (
				if "%%e"=="ALL" (
					if "%%c"=="VF" (
						echo "VF Version should build seprately" ) else (
						if "%%c"=="AY" (
						echo "AY Version should build seprately" ) else (
							set makepid=%%c
							if defined makepid (
								echo %%c@%%g>>buildallpid.txt
								for %%z in (%%g) do (
									if "%%z"=="23QUE71OD00" (
										echo "ORTHDOX Version should build seprately" ) else (
										echo [Pid] %%c@%%z>>buildallpid.txt
										if "%input_resource%"=="N" (
											echo build [Pid] %%c@%%z pac without resource
											make\make_cmd\make -r -R p=%input_project% job=4 -j2 PID=%%c@%%z br=n m=nv_parameter > err_%%c_%%zN_nv
											make\make_cmd\make -r -R p=%input_project% job=4 -j2 PID=%%c@%%z br=n image > err_%%c_%%zN_image
										) else (
											echo build [Pid] %%c@%%z pac with resource
											make\make_cmd\make -r -R p=%input_project% job=4 -j2 PID=%%c@%%z  m=nv_parameter > err_%%c_%%z_nv
											make\make_cmd\make -r -R p=%input_project% job=4 -j2 PID=%%c@%%z  m=resource_main > err_%%c_%%z_rm
											make\make_cmd\make -r -R p=%input_project% job=4 -j2 PID=%%c@%%z  image > err_%%c_%%zimage
										)
									)
								)
							)
rem makepid
						)
					)
				)
			)			
		)		
	)
)

echo this build finish
:: del /f /s /q buildallpid.txt
	


