# Microsoft Developer Studio Project File - Name="Ownership" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Ownership - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ownership.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ownership.mak" CFG="Ownership - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ownership - Win32 Debug Unicode" (based on "Win32 (x86) Application")
!MESSAGE "Ownership - Win32 Release Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Ownership"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ownership - Win32 Debug Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug Unicode"
# PROP BASE Intermediate_Dir "Debug Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug Unicode"
# PROP Intermediate_Dir "Debug Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../../../Release/Ownership/Bin/Ownership.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Ownership - Win32 Release Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release Unicode"
# PROP BASE Intermediate_Dir "Release Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release Unicode"
# PROP Intermediate_Dir "Release Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../../Release/Ownership/Bin/Ownership.exe"

!ENDIF 

# Begin Target

# Name "Ownership - Win32 Debug Unicode"
# Name "Ownership - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CityQuhaoListView.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgConvSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditCity.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGenQuhao.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGenSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoadMids.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoadText.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUpdateOwnership.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Ownership.cpp
# End Source File
# Begin Source File

SOURCE=.\Ownership.rc
# End Source File
# Begin Source File

SOURCE=.\OwnershipDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OwnershipListView.cpp
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

SOURCE=.\CityQuhaoListView.h
# End Source File
# Begin Source File

SOURCE=.\CoolStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgConvSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditCity.h
# End Source File
# Begin Source File

SOURCE=.\DlgGenQuhao.h
# End Source File
# Begin Source File

SOURCE=.\DlgGenSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoadMids.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoadText.h
# End Source File
# Begin Source File

SOURCE=.\DlgUpdateOwnership.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Ownership.h
# End Source File
# Begin Source File

SOURCE=.\OwnershipDoc.h
# End Source File
# Begin Source File

SOURCE=.\OwnershipListView.h
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

SOURCE=.\res\Add.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\del.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ownership.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ownership.rc2
# End Source File
# Begin Source File

SOURCE=.\res\OwnershipDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
