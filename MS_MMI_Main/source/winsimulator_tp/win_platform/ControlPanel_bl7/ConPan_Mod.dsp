# Microsoft Developer Studio Project File - Name="ControlPanel2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ControlPanel2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ConPan_Mod.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ConPan_Mod.mak" CFG="ControlPanel2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ControlPanel2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ControlPanel2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ControlPanel2"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ControlPanel2 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "ControlPanel2 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../../MS_Customize/export/inc" /I "../../../../../BASE/PS/export/inc" /I "../../../../../base/sim/source/test/Simconpad" /I "../../../../../base/sim/source/h" /I "../../../../../MSL/sim/inc" /I "../../../../../MSL/l4/inc" /I "../../../../../base/l4/source/win/include" /I "../../../../../MSL/layer1/inc" /I "../include" /I "../../../../../base/L4/source/arm/mnsms/h" /I "../../../../../rtos/export/inc" /I "../../../../../MSL/gas/inc" /I "../../../../../common/export/inc" /I "../../../../../base/l4/source/arm" /I "../../../../../../tools/common/include" /I "../../../../../chip_drv/export/inc" /I "../../../../../chip_drv/export/inc/outdated" /I "../../../../../ms_ref/export/inc"  /I "../../../../../MS_MMI\source\mmi_kernel\include" /I "../../../../../BASE\Layer1\export\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_CONPAN_DLL" /D "_RTOS" /D "EXTERNAL_SDL_DEBUG" /D "LOG_DEBUG" /D "SDL" /D "FLASH_EMU" /D "FEATURE_TEST" /D "_ENABLE_LOG_TOOL" /D "FEATURE_MMI_TEST" /D "MMI_TOOL" /D "CONPAN_USE_SIM_CARD" /D "CONPAN_INTERNAL_USE" /D "SIM_TEST_IN_WIN32" /D "MSCODE_MOD" /D "MULTI_SIM_SYS_QUAD" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\ControlPanel2.def" /out:"../bin/ControlPanel2.dll" /pdbtype:sept /libpath:"../bin"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ControlPanel2 - Win32 Release"
# Name "ControlPanel2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ATestPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CallConPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CBConPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CIniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ConPan.cpp
# End Source File
# Begin Source File

SOURCE=.\ConPanCfgAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\ConPanLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ConPanPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlPanel2.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlPanel2.def

!IF  "$(CFG)" == "ControlPanel2 - Win32 Release"

!ELSEIF  "$(CFG)" == "ControlPanel2 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ControlPanel2.rc
# End Source File
# Begin Source File

SOURCE=.\ExcelApplication.cpp
# End Source File
# Begin Source File

SOURCE=.\GSensorProperyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFlowPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFlowView.cpp
# End Source File
# Begin Source File

SOURCE=.\PduHexEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsConPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSINFO.cpp
# End Source File
# Begin Source File

SOURCE=.\SSConPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysInfoPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\UnicodeConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\xl5chs32.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ATestPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\CallConPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\CBConPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\CIniFile.h
# End Source File
# Begin Source File

SOURCE=.\ConPan.h
# End Source File
# Begin Source File

SOURCE=.\ConPanCfgAccess.h
# End Source File
# Begin Source File

SOURCE=.\ConPanFunc.h
# End Source File
# Begin Source File

SOURCE=.\ConPanLayer.h
# End Source File
# Begin Source File

SOURCE=.\ConPanPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\ExcelApplication.h
# End Source File
# Begin Source File

SOURCE=.\GSensorProperyPage.h
# End Source File
# Begin Source File

SOURCE=.\MsgFlowPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\MsgFlowView.h
# End Source File
# Begin Source File

SOURCE=.\PduHexEncoder.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SmsConPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\SMSINFO.h
# End Source File
# Begin Source File

SOURCE=.\SSConPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysInfoPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\UnicodeConvert.h
# End Source File
# Begin Source File

SOURCE=.\xl5chs32.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CallImage1.bmp
# End Source File
# Begin Source File

SOURCE=.\CallImage2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ControlPanel2.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
