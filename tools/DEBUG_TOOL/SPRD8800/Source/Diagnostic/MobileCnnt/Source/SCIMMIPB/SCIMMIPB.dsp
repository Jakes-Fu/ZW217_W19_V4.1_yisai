# Microsoft Developer Studio Project File - Name="SCIMMIPB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SCIMMIPB - Win32 EGO Debug_UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCIMMIPB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCIMMIPB.mak" CFG="SCIMMIPB - Win32 EGO Debug_UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCIMMIPB - Win32 Debug_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCIMMIPB - Win32 Release_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCIMMIPB - Win32 EGO Debug_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCIMMIPB - Win32 EGO Release_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SCIMMIPB"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCIMMIPB - Win32 Debug_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCIMMIPB___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "SCIMMIPB___Win32_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SCIMMIPB___Win32_Debug_UNICODE"
# PROP Intermediate_Dir "SCIMMIPB___Win32_Debug_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MTd /W3 /Gm /GX /ZI /Od /I "../Include" /I "../Application" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "PB_EXPORTS" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "PB_EXPORTS" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCIPortUD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCIMMIPB.def" /out:"../../../../Common/Bin/MobileSprite/7550/SCIMMIPB.dll" /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCIPortUD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCIMMIPB.def" /out:"../../../../Common/Bin/MobileSprite/7550/SCIMMIPB.dll" /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SCIMMIPB - Win32 Release_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCIMMIPB___Win32_Release_UNICODE0"
# PROP BASE Intermediate_Dir "SCIMMIPB___Win32_Release_UNICODE0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../Common/Lib"
# PROP Intermediate_Dir "SCIMMIPB___Win32_Release_UNICODE0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../Application" /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "PB_EXPORTS" /D "_USRDLL" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "PB_EXPORTS" /D "_USRDLL" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCIPortU.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../../Common/Bin/MobileSprite/7550/SCIMMIPB.dll" /libpath:"../../../../Common/Lib"
# ADD LINK32 SCIPortU.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\SCIMMIPB.def" /out:"../../../../Common/Bin/MobileSprite/7550/SCIMMIPB.dll" /libpath:"../../../../Common/Lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SCIMMIPB - Win32 EGO Debug_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SCIMMIPB___Win32_EGO_Debug_UNICODE0"
# PROP BASE Intermediate_Dir "SCIMMIPB___Win32_EGO_Debug_UNICODE0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../Common/Lib"
# PROP Intermediate_Dir "SCIMMIPB___Win32_EGO_Debug_UNICODE0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MTd /W3 /Gm /GX /ZI /Od /I "../../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "PB_EXPORTS" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "../../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "PB_EXPORTS" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCIPortUD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCIMMIPB.def" /out:"../../../../Common/Bin/eGo/SCIMMIPBD.dll" /implib:"../../../../Common/Lib/SCIMMIPBD.lib" /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SCIPortUD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\SCIMMIPBUD.def" /out:"../../../../Common/Bin/eGo/SCIMMIPBUD.dll" /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SCIMMIPB - Win32 EGO Release_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SCIMMIPB___Win32_EGO_Release_UNICODE0"
# PROP BASE Intermediate_Dir "SCIMMIPB___Win32_EGO_Release_UNICODE0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../Common/Lib"
# PROP Intermediate_Dir "SCIMMIPB___Win32_EGO_Release_UNICODE0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "PB_EXPORTS" /D "_USRDLL" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "PB_EXPORTS" /D "_USRDLL" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SCIPortU.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../../../Common/Bin/eGo/SCIMMIPB.dll" /libpath:"../../../../Common/Lib"
# ADD LINK32 SCIPortU.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\SCIMMIPBU.def" /out:"../../../../Release/eGo/bin/SCIMMIPBU.dll" /libpath:"../../../../Common/Lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SCIMMIPB - Win32 Debug_UNICODE"
# Name "SCIMMIPB - Win32 Release_UNICODE"
# Name "SCIMMIPB - Win32 EGO Debug_UNICODE"
# Name "SCIMMIPB - Win32 EGO Release_UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Converter.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\PB.cpp
# End Source File
# Begin Source File

SOURCE=.\SCIMMIPB.cpp
# End Source File
# Begin Source File

SOURCE=.\SCIMMIPB.def

!IF  "$(CFG)" == "SCIMMIPB - Win32 Debug_UNICODE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCIMMIPB - Win32 Release_UNICODE"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCIMMIPB - Win32 EGO Debug_UNICODE"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SCIMMIPB - Win32 EGO Release_UNICODE"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SCIMMIPB.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Converter.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\PB.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SCIMMIPB.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SCIMMIPB.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
