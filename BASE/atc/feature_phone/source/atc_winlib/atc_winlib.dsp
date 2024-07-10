# Microsoft Developer Studio Project File - Name="atc_winlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=atc_winlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "atc_winlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "atc_winlib.mak" CFG="atc_winlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "atc_winlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "atc_winlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "atc_winlib"
# PROP Scc_LocalPath "\MS_Code\ATC\Source"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "atc_winlib - Win32 Release"

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

!ELSEIF  "$(CFG)" == "atc_winlib - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /X /I "../h" /I "../../../rtos/include" /I "../../../common/include" /I "../../../ps/include" /I "../../../ref/include" /I "../../../ps/source/common/h" /I "../../../ps/source/dl/h" /I "../../../ps/source/nvitem_ps/h" /I "../../../ps/source/cc/h" /I "../../../ps/source/rr/h" /I "../../../ps/source/mac/h" /I "../../../ps/source/mm/h" /I "../../../ps/source/rlc/h" /I "../../../ps/source/llc/h" /I "../../../ps/source/gmm/h" /I "../../../ps/source/sndcp/h" /I "../../../ps/source/sms/h" /I "../../../ps/source/ss/h" /I "../../../ps/source/sm/h" /I "../../../ps/source/sct/h" /I "../../../ps/source/mn/h" /I "../../../ps/source/mn/call/h" /I "../../../ps/source/mn/datamag/h" /I "../../../ps/source/mn/gprs/h" /I "../../../ps/source/mn/mnsms/h" /I "../../../ps/source/mn/mnsmscb/h" /I "../../../ps/source/mn/mnss/h" /I "../../../ps/source/mn/phone/h" /I "../../../ps/source/sim/h" /I "../../../ps/source/L1_simulator/h" /I "../../../ps/source/smscb/h" /I "../../../tcpip/source/h" /I "../../../tcpip/source/win32_threadx" /I "../include" /I "../../../tools/diagnostic/mssim/source/mskernel/include" /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "..\..\..\..\Tools\Diagnostic\MSSim\Source\MSBridge" /I "..\..\..\..\Tools\Diagnostic\MSSim\Source\MSKernel" /I "..\..\..\Layer1\Include" /I "..\..\..\mmi\include" /I "..\..\..\..\Tools\Diagnostic\MSDev\Source\MSKernel\Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_RTOS" /D "__ATC_DPHONE" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_RTOS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\atc_winlib.lib"

!ENDIF 

# Begin Target

# Name "atc_winlib - Win32 Release"
# Name "atc_winlib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\c\atc.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_basic_cmd.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_common.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_gsm.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_gsm_sms.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_gsm_ss.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_info.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_lex.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_main.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_malloc.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_phonebook.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_plus_gprs.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_test_WIN32.c
# End Source File
# Begin Source File

SOURCE=..\c\atc_yacc.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
