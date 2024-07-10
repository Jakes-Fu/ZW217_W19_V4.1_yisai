# Microsoft Developer Studio Project File - Name="FontTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FontTool - Win32 Cust Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FontTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FontTool.mak" CFG="FontTool - Win32 Cust Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FontTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FontTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "FontTool - Win32 Cust Release" (based on "Win32 (x86) Application")
!MESSAGE "FontTool - Win32 Cust Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "FontTool"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FontTool - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../Include" /I "../freetypelib/Include" /I "../spml" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 freetype.lib spml.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../../../Release/FontTool/Bin/FontTool.exe" /libpath:"../lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "FontTool - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "../Include" /I "../freetypelib/Include" /I "../spml" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 freetyped.lib spmld.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../../../Release/FontTool/Bin/FontTool.exe" /pdbtype:sept /libpath:"../lib"

!ELSEIF  "$(CFG)" == "FontTool - Win32 Cust Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FontTool___Win32_Cust_Release"
# PROP BASE Intermediate_Dir "FontTool___Win32_Cust_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FontTool___Win32_Cust_Release"
# PROP Intermediate_Dir "FontTool___Win32_Cust_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../Include" /I "../freetypelib/Include" /I "../spml" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 freetype.lib spml.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../../../Common/Bin/FontTool/FontTool.exe" /libpath:"../lib" /IGNORE:4089
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 freetype.lib spml.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../Bin/FontTool.exe" /libpath:"../lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "FontTool - Win32 Cust Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FontTool___Win32_Cust_Debug"
# PROP BASE Intermediate_Dir "FontTool___Win32_Cust_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FontTool___Win32_Cust_Debug"
# PROP Intermediate_Dir "FontTool___Win32_Cust_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "../Include" /I "../freetypelib/Include" /I "../spml" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 freetyped.lib spmld.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../../../Common/Bin/FontTool/FontTool.exe" /pdbtype:sept /libpath:"../lib"
# ADD LINK32 freetyped.lib spmld.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../Bin/FontTool.exe" /pdbtype:sept /libpath:"../lib"

!ENDIF 

# Begin Target

# Name "FontTool - Win32 Release"
# Name "FontTool - Win32 Debug"
# Name "FontTool - Win32 Cust Release"
# Name "FontTool - Win32 Cust Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CoolButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAdjustParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAdvSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOTFSeting.cpp
# End Source File
# Begin Source File

SOURCE=.\EditFontDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Font_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\FontTool.cpp
# End Source File
# Begin Source File

SOURCE=.\FontTool.rc
# End Source File
# Begin Source File

SOURCE=.\FontToolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GlyphData.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CoolButton.h
# End Source File
# Begin Source File

SOURCE=.\CoolListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CoolStatic.h
# End Source File
# Begin Source File

SOURCE=.\DlgAdjustParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgAdvSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgOTFSeting.h
# End Source File
# Begin Source File

SOURCE=.\EditFontDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Font_Interface.h
# End Source File
# Begin Source File

SOURCE=.\FontTool.h
# End Source File
# Begin Source File

SOURCE=.\FontToolDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\Global.h
# End Source File
# Begin Source File

SOURCE=.\GlyphData.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap_pen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_erase.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_select.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_undo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor_e.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_p.cur
# End Source File
# Begin Source File

SOURCE=.\res\FontTool.ico
# End Source File
# Begin Source File

SOURCE=.\res\FontTool.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
