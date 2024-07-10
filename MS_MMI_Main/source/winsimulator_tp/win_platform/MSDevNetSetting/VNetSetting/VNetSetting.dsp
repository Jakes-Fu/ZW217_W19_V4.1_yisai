# Microsoft Developer Studio Project File - Name="VNetSetting" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VNetSetting - Win32 Self Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VNetSetting.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VNetSetting.mak" CFG="VNetSetting - Win32 Self Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VNetSetting - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VNetSetting - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VNetSetting - Win32 Self Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "VNetSetting"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VNetSetting - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\include" /I ".\include\net" /I ".\include\winpcap" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 IPHlpApi.Lib ws2_32.lib packet.lib wininet.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../VNetTest/Release/VNetSetting.dll" /libpath:".\library\net" /libpath:".\library\winpcap"

!ELSEIF  "$(CFG)" == "VNetSetting - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\include\net" /I ".\include\winpcap" /I "..\..\..\..\..\..\RTOS\export\inc" /I "..\..\..\..\..\..\common\export\inc" /I "..\..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_RTOS" /D "_MBCS" /D "_USRDLL" /D "HAVE_REMOTE" /D "WPCAP" /D "MSCODE_MOD" /D "_ENABLE_LOG_TOOL" /D "EXTERNAL_SDL_DEBUG" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nafxcwd.lib libcmtd.lib IPHlpApi.Lib ws2_32.lib packet.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\VNetSetting.dll" /pdbtype:sept /libpath:".\library\net" /libpath:".\library\winpcap"

!ELSEIF  "$(CFG)" == "VNetSetting - Win32 Self Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VNetSetting___Win32_Self_Debug"
# PROP BASE Intermediate_Dir "VNetSetting___Win32_Self_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "VNetSetting___Win32_Self_Debug"
# PROP Intermediate_Dir "VNetSetting___Win32_Self_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\include\net" /I ".\include\winpcap" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "HAVE_REMOTE" /D "WPCAP" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\include\net" /I ".\include\winpcap" /I "..\..\..\..\..\..\Ms_Ref\export\inc" /I "..\..\..\..\..\..\RTOS\export\inc" /I "..\..\..\..\..\..\common\export\inc" /I "..\..\..\..\..\..\DAPS\export\inc" /I "..\..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "HAVE_REMOTE" /D "WPCAP" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 IPHlpApi.Lib ws2_32.lib packet.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../VNetTest/Debug/VNetSetting.dll" /pdbtype:sept /libpath:".\library\net" /libpath:".\library\winpcap"
# ADD LINK32 IPHlpApi.Lib ws2_32.lib packet.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\VNetTest\Debug\VNetSetting.dll" /pdbtype:sept /libpath:".\library\net" /libpath:".\library\winpcap"

!ENDIF 

# Begin Target

# Name "VNetSetting - Win32 Release"
# Name "VNetSetting - Win32 Debug"
# Name "VNetSetting - Win32 Self Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CloseWaitDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\FeatureDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\IpSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IpTestingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MacTestingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VNetSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\VNetSetting.def
# End Source File
# Begin Source File

SOURCE=.\VNetSetting.rc
# End Source File
# Begin Source File

SOURCE=.\VNetSettingDLL.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CloseWaitDlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureDll.h
# End Source File
# Begin Source File

SOURCE=.\IpSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\IpTestingDlg.h
# End Source File
# Begin Source File

SOURCE=.\MacTestingDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VNetSetting.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Cancel.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ok.ico
# End Source File
# Begin Source File

SOURCE=.\res\VNetSetting.rc2
# End Source File
# End Group
# Begin Group "MSDEV NetPanal"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NetsettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NetsettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\NetstateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NetstateDlg.h
# End Source File
# End Group
# Begin Group "Utility"

# PROP Default_Filter ""
# Begin Group "ButtonST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CButtonST\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\CButtonST\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\CButtonST\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CButtonST\BtnST.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\common.h
# End Source File
# Begin Source File

SOURCE=.\source\config_file.cpp
# End Source File
# Begin Source File

SOURCE=.\include\config_file.h
# End Source File
# Begin Source File

SOURCE=.\source\file_path.cpp
# End Source File
# Begin Source File

SOURCE=.\include\file_path.h
# End Source File
# Begin Source File

SOURCE=.\source\HexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\include\HexEdit.h
# End Source File
# Begin Source File

SOURCE=.\source\msdev_param.cpp
# End Source File
# Begin Source File

SOURCE=.\include\msdev_param.h
# End Source File
# Begin Source File

SOURCE=.\source\msnet_param.cpp
# End Source File
# Begin Source File

SOURCE=.\include\msnet_param.h
# End Source File
# Begin Source File

SOURCE=.\source\net_driver.cpp
# End Source File
# Begin Source File

SOURCE=.\include\net_driver.h
# End Source File
# Begin Source File

SOURCE=.\source\net_test.cpp
# End Source File
# Begin Source File

SOURCE=.\include\net_test.h
# End Source File
# Begin Source File

SOURCE=.\source\RegOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\include\RegOpt.h
# End Source File
# End Group
# Begin Group "Control"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
