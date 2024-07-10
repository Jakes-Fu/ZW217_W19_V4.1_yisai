# Microsoft Developer Studio Project File - Name="Filesys_DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Filesys_DLL - Win32 Debug_UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Filesys_DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Filesys_DLL.mak" CFG="Filesys_DLL - Win32 Debug_UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Filesys_DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Filesys_DLL - Win32 Debug_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Filesys_DLL"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Filesys_DLL - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CTRLPAN_DLL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CTRLPAN_DLL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "Filesys_DLL - Win32 Debug_UNICODE"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Filesys_DLL___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "Filesys_DLL___Win32_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Filesys_DLL___Win32_Debug_UNICODE"
# PROP Intermediate_Dir "Filesys_DLL___Win32_Debug_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\..\WIN\include\os" /I "..\..\..\..\RTOS\Include" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CTRLPAN_DLL_EXPORTS" /D "_WINDLL" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /Zp4 /MTd /W3 /Gm /GX /ZI /Od /I "..\MSKernel\Include" /I "..\..\..\..\..\MS_Code\Ms_Ref\export\inc" /I "..\..\..\..\..\MS_Code\RTOS\Include\\" /I "..\..\..\..\..\MS_Code\Ref\Include" /I "..\..\..\..\..\MS_Code\Transform\Export\inc\\" /I "..\..\..\..\..\MS_Code\common\export\inc" /I "..\..\..\..\..\MS_Code\rtos\export\inc" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CTRLPAN_DLL_EXPORTS" /D "_WINDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /entry:"wWinMainCRTStartup" /dll /debug /machine:I386 /out:"../Bin/filesys.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"G:\apple.gao_msdev_sview\MS_Code\build\sc6600l_sp6601l_builddir\win\bin\filesys.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Filesys_DLL - Win32 Release"
# Name "Filesys_DLL - Win32 Debug_UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CtrlPan_DLL.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlPan_DLL.rc
# End Source File
# Begin Source File

SOURCE=.\FileSysDlag.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CtrlPan_DLL.h
# End Source File
# Begin Source File

SOURCE=.\CtrlPanFuns.h
# End Source File
# Begin Source File

SOURCE=.\FileSysDlag.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\sfs_func_def.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\Lib\MSDevKernel.lib
# End Source File
# End Target
# End Project
