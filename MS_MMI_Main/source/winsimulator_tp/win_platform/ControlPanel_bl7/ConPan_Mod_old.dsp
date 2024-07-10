# Microsoft Developer Studio Project File - Name="ControlPanel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ControlPanel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ConPan_Mod.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ConPan_Mod.mak" CFG="ControlPanel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ControlPanel - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ControlPanel - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ControlPanel"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ControlPanel - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "ControlPanel - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../common/Lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W3 /Gm /GX /ZI /Od /I "../../../../../ms_code/sim/source/test/Simconpad" /I "../../../../../ms_code/mm_ref/source/drv_sc6800/h" /I "../../../../../ms_code/sim/source/h" /I "../../../../../ms_code/sim/export/inc" /I "../../../../../ms_code/l4/export/inc" /I "../../../../../ms_code/l4/source/win/include" /I "../../../../../ms_code/layer1/export/inc" /I "../../../../../ms_code/transform/export/inc" /I "../mskernel/include" /I "../../../../../ms_code/L4/source/arm/mnsms/h" /I "../../../../../ms_code/mm_Ref/include" /I "../../../../../ms_code/rtos/export/inc" /I "../../../../../ms_code/ps/export/inc" /I "../../../../../ms_code/common/export/inc" /I "../../../../../ms_code/l4/source/arm" /I "../../../../common/include" /I "../../../../../ms_code/chip_drv/export/inc" /I "../../../../../MS_Code\MS_MMI\source\winsimulator_tp\win_platform\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_CONPAN_DLL" /D "_USRDLL" /D "_MBCS" /D "_RTOS" /D "EXTERNAL_SDL_DEBUG" /D "LOG_DEBUG" /D "SDL" /D "FLASH_EMU" /D "FEATURE_TEST" /D "_ENABLE_LOG_TOOL" /D "FEATURE_MMI_TEST" /D "MMI_TOOL" /D "CONPAN_USE_SIM_CARD" /D "CONPAN_INTERNAL_USE" /D "SIM_TEST_IN_WIN32" /D "_WINDLL" /D "MSCODE_MOD" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCD.lib" /out:"../../../../common/bin/ConPan.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ControlPanel - Win32 Release"
# Name "ControlPanel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=.\ConPan.rc
# End Source File
# Begin Source File

SOURCE=.\ConPanLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ConPanPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFlowPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFlowView.cpp
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
# End Source File
# Begin Source File

SOURCE=.\SysInfoPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\UnicodeConvert.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=.\ConPanFunc.h
# End Source File
# Begin Source File

SOURCE=.\ConPanLayer.h
# End Source File
# Begin Source File

SOURCE=.\ConPanPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\MsgFlowPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\MsgFlowView.h
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
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CallImage1.bmp
# End Source File
# Begin Source File

SOURCE=.\CallImage2.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
