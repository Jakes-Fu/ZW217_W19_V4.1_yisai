# Microsoft Developer Studio Project File - Name="PS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "atcwin_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "atcwin_lib.mak" CFG="PS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PS - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PS - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "PS"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /W3 /Gm /GX /ZI /Od /I "../../layer1/include" /I "../../common/include" /I "../../rtos/include" /I "./common/h" /I "./dl/h" /I "./nvitem_ps/h" /I "./cc/h" /I "./rr/h" /I "./mac/h" /I "../../ref/include" /I "./mm/h" /I "./rlc/h" /I "./llc/h" /I "./gmm/h" /I "./sndcp/h" /I "./sms/h" /I "./ss/h" /I "./atc/h" /I "./sm/h" /I "./sct/h" /I "./mn/h" /I "./mn/call/h" /I "./mn/datamag/h" /I "./mn/gprs/h" /I "./mn/mnsms/h" /I "./mn/mnsmscb/h" /I "./mn/mnss/h" /I "./mn/phone/h" /I "./sim/h" /I "./L1_simulator/h" /I "./smscb/h" /I "../../tcpip/source/h" /I "../../tcpip/source/win32_threadx" /I "../include" /I "../../../Tools/Diagnostic/MSSim/Source/msbridge" /I "../../../Tools/Diagnostic/MSSim/Source/mskernel" /I "../../mmi/include" /I "./../Source/h" /I "..\..\PS\Include" /I "..\..\PS\Source\MN\CALL\h" /I "..\..\PS\Source\SIM\h" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_RTOS" /D "_FEATURE_BSSIM_TEST" /D "EXTERNAL_SDL_DEBUG" /D "LOG_DEBUG" /D "_TEMP_DEBUG" /D "SDL" /D "_MN_EVENT" /D "FLASH_EMU" /D "_MSSIM" /D "FEATURE_TEST" /D "_ENABLE_WATCHDOG" /D "SIM_TEST_IN_WIN32" /D "_ENABLE_LOG_TOOL" /D "_SUPPORT_GPRS_" /D "MEMORY_DEBUG_CODE" /D "LOG_FLOW_CONTROL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /i "..\..\MS_code\layer1\include\\" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/atcwin_lib.lib"

!ENDIF 

# Begin Target

# Name "PS - Win32 Release"
# Name "PS - Win32 Debug"
# Begin Group "atc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\c\atc.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_basic_cmd.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_common.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm_sms.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm_ss.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_gsm_stk.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_info.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_lex.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_malloc.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_phonebook.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_plus_gprs.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_public_phone.c
# End Source File
# Begin Source File

SOURCE=.\c\atc_yacc.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\c\atc_main.c
# End Source File
# End Target
# End Project
