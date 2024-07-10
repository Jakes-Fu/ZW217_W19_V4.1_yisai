# Microsoft Developer Studio Project File - Name="ChannelServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ChannelServer - Win32 Cust Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ChannelServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ChannelServer.mak" CFG="ChannelServer - Win32 Cust Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ChannelServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ChannelServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ChannelServer - Win32 Cust Debug" (based on "Win32 (x86) Application")
!MESSAGE "ChannelServer - Win32 Cust Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ChannelServer"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ChannelServer - Win32 Release"

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
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "..\Include" /I "../../../../common/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Channel.lib DiagChan.lib version.lib Ws2_32.lib porthound.lib CrashReport.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:"../../../../common/bin/ChannelServer.pdb" /map:"../../../../Common/bin/ChannelServer.map" /debug /machine:I386 /out:"../../../../common/bin/ChannelServer.exe" /libpath:"../../../../Common/Lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetDir=\Work\workspace\tools-main\trunk\common\bin
TargetPath=\Work\workspace\tools-main\trunk\common\bin\ChannelServer.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy /Y $(TargetPath) ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\DiagChan.dll ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\Channel.dll ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\CrashReport.dll ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\DiagChan.pdb ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\DiagChan.map ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\Channel.pdb ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\Channel.map ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\ChannelServer.pdb ..\..\..\..\Release\Channelserver\Bin	xcopy /Y $(TargetDir)\ChannelServer.map ..\..\..\..\Release\Channelserver\Bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ChannelServer - Win32 Debug"

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
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "..\Include" /I "../../../../common/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"c:/sw_temp/debug/ChannelServer.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ChannelD.lib DiagChand.lib version.lib Ws2_32.lib porthoundd.lib CrashReportd.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:"../../../../common/bin/ChannelServerd.pdb" /map:"../../../../Common/bin/ChannelServerd.map" /debug /machine:I386 /out:"../../../../Common/bin/ChannelServerd.exe" /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetDir=\Work\workspace\tools-main\trunk\Common\bin
TargetPath=\Work\workspace\tools-main\trunk\Common\bin\ChannelServerd.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy /Y $(TargetPath) ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\DiagChand.dll ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\Channeld.dll ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\CrashReportd.dll ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\DiagChand.map ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\ChannelServerd.map ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\ChannelServerd.pdb ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\DiagChand.pdb ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\Channeld.map ..\..\..\..\Release\Channelserver\Source\Debug	xcopy /Y $(TargetDir)\Channeld.pdb ..\..\..\..\Release\Channelserver\Source\Debug
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ChannelServer - Win32 Cust Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ChannelServer___Win32_Cust_Debug"
# PROP BASE Intermediate_Dir "ChannelServer___Win32_Cust_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ChannelServer___Win32_Cust_Debug"
# PROP Intermediate_Dir "ChannelServer___Win32_Cust_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"c:/sw_temp/debug/ChannelServer.bsc"
# ADD BSC32 /nologo /o"c:/sw_temp/debug/ChannelServer.bsc"
LINK32=link.exe
# ADD BASE LINK32 ChannelD.lib DiagChand.lib version.lib Ws2_32.lib setupapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:"c:/sw_temp/debug/ChannelServer.pdb" /debug /machine:I386 /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT BASE LINK32 /pdb:none /incremental:no /map /force
# ADD LINK32 ChannelD.lib DiagChand.lib version.lib Ws2_32.lib porthoundd.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:"c:/sw_temp/debug/ChannelServer.pdb" /debug /machine:I386 /out:"..\Debug\ChannelServer.exe" /pdbtype:sept /libpath:"..\lib"
# SUBTRACT LINK32 /pdb:none /incremental:no /map /force

!ELSEIF  "$(CFG)" == "ChannelServer - Win32 Cust Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ChannelServer___Win32_Cust_Release"
# PROP BASE Intermediate_Dir "ChannelServer___Win32_Cust_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ChannelServer___Win32_Cust_Release"
# PROP Intermediate_Dir "ChannelServer___Win32_Cust_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Channel.lib DiagChan.lib version.lib Ws2_32.lib setupapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../../common/bin/ChannelServer.exe" /libpath:"../../../../Common/Lib"
# ADD LINK32 Channel.lib DiagChan.lib version.lib Ws2_32.lib porthound.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../bin/ChannelServer.exe" /libpath:"..\lib"

!ENDIF 

# Begin Target

# Name "ChannelServer - Win32 Release"
# Name "ChannelServer - Win32 Debug"
# Name "ChannelServer - Win32 Cust Debug"
# Name "ChannelServer - Win32 Cust Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AllPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelServer.rc
# End Source File
# Begin Source File

SOURCE=.\ChannelServerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelServerView.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientInfoSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigureSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlCenter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFileConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSocketConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUartConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\dlmalloc.c

!IF  "$(CFG)" == "ChannelServer - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ChannelServer - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ChannelServer - Win32 Cust Debug"

!ELSEIF  "$(CFG)" == "ChannelServer - Win32 Cust Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Function.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SpLog.cpp
# SUBTRACT CPP /YX /Yc /Yu
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

SOURCE=.\AllPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\ChannelServer.h
# End Source File
# Begin Source File

SOURCE=.\ChannelServerDoc.h
# End Source File
# Begin Source File

SOURCE=.\ChannelServerView.h
# End Source File
# Begin Source File

SOURCE=.\ClientInfoSheet.h
# End Source File
# Begin Source File

SOURCE=.\ConfigureSheet.h
# End Source File
# Begin Source File

SOURCE=.\ControlCenter.h
# End Source File
# Begin Source File

SOURCE=.\DlgFileConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgSocketConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgUartConfig.h
# End Source File
# Begin Source File

SOURCE=.\dlmalloc.h
# End Source File
# Begin Source File

SOURCE=.\Function.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\ISpLog.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SpLog.h
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

SOURCE=.\res\active_l.ico
# End Source File
# Begin Source File

SOURCE=.\res\active_r.ico
# End Source File
# Begin Source File

SOURCE=.\res\ChannelServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\ChannelServer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ChannelServerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\High.ico
# End Source File
# Begin Source File

SOURCE=.\res\Higher.ico
# End Source File
# Begin Source File

SOURCE=.\res\Highest.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\inactive.ico
# End Source File
# Begin Source File

SOURCE=.\res\Low.ico
# End Source File
# Begin Source File

SOURCE=.\res\Lower.ico
# End Source File
# Begin Source File

SOURCE=.\res\Lowest.ico
# End Source File
# Begin Source File

SOURCE=.\res\Middle.ico
# End Source File
# Begin Source File

SOURCE=.\res\upalert.ico
# End Source File
# Begin Source File

SOURCE=.\res\updownalert.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
