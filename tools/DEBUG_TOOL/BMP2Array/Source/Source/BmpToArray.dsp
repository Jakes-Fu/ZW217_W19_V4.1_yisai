# Microsoft Developer Studio Project File - Name="BmpToArray" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BmpToArray - Win32 Cust Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BmpToArray.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BmpToArray.mak" CFG="BmpToArray - Win32 Cust Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BmpToArray - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BmpToArray - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "BmpToArray - Win32 Cust Release" (based on "Win32 (x86) Application")
!MESSAGE "BmpToArray - Win32 Cust Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BmpToArray"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BmpToArray - Win32 Release"

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
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\include" /I "..\3D" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_WIN_LOG_TOOL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 X_IMG.lib SciDllProducer.lib ImgOptm.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../Common/Bin/MMITools/BmpToArray.exe" /libpath:"../../../Common/lib"

!ELSEIF  "$(CFG)" == "BmpToArray - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\include" /I "..\3D" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_WIN_LOG_TOOL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 X_IMGD.lib SciDllProducer.lib ImgOptm.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../../Common/Bin/MMITools/BmpToArray.exe" /pdbtype:sept /libpath:"../../../Common/lib"

!ELSEIF  "$(CFG)" == "BmpToArray - Win32 Cust Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BmpToArray___Win32_Cust_Release"
# PROP BASE Intermediate_Dir "BmpToArray___Win32_Cust_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BmpToArray___Win32_Cust_Release"
# PROP Intermediate_Dir "BmpToArray___Win32_Cust_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "..\include" /I "..\3D" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\include" /I "..\3D" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 X_IMG.lib SciDllProducer.lib ImgOptm.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../Common/Bin/MMITools/BmpToArray.exe" /libpath:"../../../Common/lib"
# ADD LINK32 X_IMG.lib SciDllProducer.lib ImgOptm.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../Bin/BmpToArray.exe" /libpath:"../lib"

!ELSEIF  "$(CFG)" == "BmpToArray - Win32 Cust Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BmpToArray___Win32_Cust_Debug"
# PROP BASE Intermediate_Dir "BmpToArray___Win32_Cust_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BmpToArray___Win32_Cust_Debug"
# PROP Intermediate_Dir "BmpToArray___Win32_Cust_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\include" /I "..\3D" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\include" /I "..\3D" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 X_IMGD.lib SciDllProducer.lib ImgOptm.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../../Common/Bin/MMITools/BmpToArray.exe" /pdbtype:sept /libpath:"../../../Common/lib"
# ADD LINK32 X_IMG.lib SciDllProducer.lib ImgOptm.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../Bin/BmpToArray.exe" /pdbtype:sept /libpath:"../lib"

!ENDIF 

# Begin Target

# Name "BmpToArray - Win32 Release"
# Name "BmpToArray - Win32 Debug"
# Name "BmpToArray - Win32 Cust Release"
# Name "BmpToArray - Win32 Cust Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BmpToArray.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpToArray.rc
# End Source File
# Begin Source File

SOURCE=.\BmpToArrayDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpToArrayView.cpp
# End Source File
# Begin Source File

SOURCE=.\Converter.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg3D.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOSD.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Generater.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OsdGenerater.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XTabCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BmpToArray.h
# End Source File
# Begin Source File

SOURCE=.\BmpToArrayDoc.h
# End Source File
# Begin Source File

SOURCE=.\BmpToArrayView.h
# End Source File
# Begin Source File

SOURCE=.\Converter.h
# End Source File
# Begin Source File

SOURCE=.\CoolStatic.h
# End Source File
# Begin Source File

SOURCE=.\Dlg3D.h
# End Source File
# Begin Source File

SOURCE=.\DlgMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgOSD.h
# End Source File
# Begin Source File

SOURCE=.\FileDlg.h
# End Source File
# Begin Source File

SOURCE=.\Generater.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OsdGenerater.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VerInfo.h
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\XTabCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BmpToArray.ico
# End Source File
# Begin Source File

SOURCE=.\res\BmpToArray.rc2
# End Source File
# Begin Source File

SOURCE=.\res\BmpToArrayDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\3D\se_table_gen.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
