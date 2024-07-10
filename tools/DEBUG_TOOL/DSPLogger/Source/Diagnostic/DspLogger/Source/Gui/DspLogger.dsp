# Microsoft Developer Studio Project File - Name="DspLogger" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DspLogger - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DspLogger.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DspLogger.mak" CFG="DspLogger - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DspLogger - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DspLogger - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "DspLogger - Win32 Release_Audio" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "DspLogger"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DspLogger - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Common\Bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GR /GX /Zi /O2 /I "../DspKernel" /I "../RTDisp" /I "../Graph" /I "..\LUA" /I "..\include" /I "..\..\..\..\Common\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib lua51.lib CrashReport.lib Ws2_32.lib /nologo /subsystem:windows /pdb:"..\..\..\..\Release\DSPLogger\Bin\DspLogger.pdb" /debug /machine:I386 /libpath:"..\..\..\..\Common\Lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\Diagnostic\DspLogger\Documents\Specifications\DspLogger User Guide.doc" "..\..\..\..\Release\DSPLogger\Doc\DspLogger User Guide.doc"	copy "..\..\..\..\Diagnostic\DspLogger\Documents\En\DSPLogger User Guide(en).doc" "..\..\..\..\Release\DSPLogger\Doc\DSPLogger User Guide(en).doc"	copy "..\..\..\..\common\bin\Channel.dll" "..\..\..\..\Release\DSPLogger\Bin\Channel.dll"	copy "..\..\..\..\common\bin\DiagChan.dll" "..\..\..\..\Release\DSPLogger\Bin\DiagChan.dll"	copy "..\..\..\..\common\bin\DspKernel.dll" "..\..\..\..\Release\DSPLogger\Bin\DspKernel.dll"	copy "..\..\..\..\common\bin\Dsplogger.dat" "..\..\..\..\Release\DSPLogger\Bin\Dsplogger.dat"	copy "..\..\..\..\common\bin\DspLogger.exe" "..\..\..\..\Release\DSPLogger\Bin\DspLogger.exe"	copy "..\..\..\..\common\bin\SciUartComm.dll" "..\..\..\..\Release\DSPLogger\Bin\SciUartComm.dll"	copy "..\..\..\..\common\bin\CrashReport.dll" "..\..\..\..\Release\DSPLogger\Bin\CrashReport.dll"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DspLogger - Win32 Debug"

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
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GR /GX /ZI /Od /I "../DspKernel" /I "../RTDisp" /I "../Graph" /I "..\LUA" /I "..\include" /I "..\..\..\..\Common\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "APSTUDIO_INVOKED" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib lua51.lib CrashReportd.lib Ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\Release\DSPLogger\Bin\DspLogger.exe" /pdbtype:sept /libpath:"..\..\..\..\Common\Lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\common\bin\Channeld.dll" "..\..\..\..\Release\DSPLogger\Bin\Channeld.dll"	copy "..\..\..\..\common\bin\DiagChand.dll" "..\..\..\..\Release\DSPLogger\Bin\DiagChand.dll"	copy "..\..\..\..\common\bin\DspKerneld.dll" "..\..\..\..\Release\DSPLogger\Bin\DspKerneld.dll"	copy "..\..\..\..\common\bin\SciUartCommd.dll" "..\..\..\..\Release\DSPLogger\Bin\SciUartCommd.dll"	copy "..\..\..\..\common\bin\CrashReportd.dll" "..\..\..\..\Release\DSPLogger\Bin\CrashReportd.dll"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DspLogger - Win32 Release_Audio"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DspLogger___Win32_Release_Audio"
# PROP BASE Intermediate_Dir "DspLogger___Win32_Release_Audio"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DspLogger___Win32_Release_Audio"
# PROP Intermediate_Dir "DspLogger___Win32_Release_Audio"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MT /W4 /GR /GX /O2 /I "../DspKernel" /D "DSPLOGGER_2p5G" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GR /GX /O2 /I "../DspKernel" /D "DSPLOGGER_AUDIO" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\..\..\Common\Bin\Dsplogger_2p5G/DspLogger.exe"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\..\..\Common\Bin\Dsplogger_Audio\DspLogger.exe"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "DspLogger - Win32 Release"
# Name "DspLogger - Win32 Debug"
# Name "DspLogger - Win32 Release_Audio"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BasicChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelImpuleStc.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsDiagStc.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DevHound.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindTP.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImport.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlaybackOrg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSaveSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DspLogger.cpp
# End Source File
# Begin Source File

SOURCE=.\DspLogger.rc
# End Source File
# Begin Source File

SOURCE=.\DspLoggerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\eval.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterChartCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterChartView.cpp
# End Source File
# Begin Source File

SOURCE=.\filterdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterMainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChartCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChartView.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChartView_Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChartView_DrawTogether.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChartView_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\FindDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FindFindView.cpp
# End Source File
# Begin Source File

SOURCE=.\FindLeftWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FindListView.cpp
# End Source File
# Begin Source File

SOURCE=.\FindMainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\FindRightWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FindTpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FmlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GotoItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyInfoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyInfoFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyinfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkSet.cpp
# End Source File
# Begin Source File

SOURCE=.\MathFormula.cpp
# End Source File
# Begin Source File

SOURCE=.\MruComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseOrgExFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ReplaySetting.cpp
# End Source File
# Begin Source File

SOURCE=.\SetComDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SizingControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TestPointsView.cpp
# End Source File
# Begin Source File

SOURCE=.\TPAddrDescDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TPAddrDescItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BasicChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChannelImpuleStc.h
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\ConsDiagStc.h
# End Source File
# Begin Source File

SOURCE=.\CoolBar.h
# End Source File
# Begin Source File

SOURCE=.\DevHound.h
# End Source File
# Begin Source File

SOURCE=.\DlgFindTP.h
# End Source File
# Begin Source File

SOURCE=.\DlgImport.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlaybackOrg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSaveSetting.h
# End Source File
# Begin Source File

SOURCE=.\DspLogger.h
# End Source File
# Begin Source File

SOURCE=.\DspLoggerDoc.h
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.h
# End Source File
# Begin Source File

SOURCE=.\eval.h
# End Source File
# Begin Source File

SOURCE=.\ExportDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileDlg.h
# End Source File
# Begin Source File

SOURCE=.\Filter.h
# End Source File
# Begin Source File

SOURCE=.\FilterChartCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FilterChartView.h
# End Source File
# Begin Source File

SOURCE=.\filterdoc.h
# End Source File
# Begin Source File

SOURCE=.\FilterMainFrame.h
# End Source File
# Begin Source File

SOURCE=.\FindChartCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FindChartView.h
# End Source File
# Begin Source File

SOURCE=.\FindDoc.h
# End Source File
# Begin Source File

SOURCE=.\FindFindView.h
# End Source File
# Begin Source File

SOURCE=.\FindLeftWnd.h
# End Source File
# Begin Source File

SOURCE=.\FindListView.h
# End Source File
# Begin Source File

SOURCE=.\FindMainFrame.h
# End Source File
# Begin Source File

SOURCE=.\FindRightWnd.h
# End Source File
# Begin Source File

SOURCE=.\FindTpDlg.h
# End Source File
# Begin Source File

SOURCE=.\FmlDlg.h
# End Source File
# Begin Source File

SOURCE=.\GotoItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\KeyInfoDoc.h
# End Source File
# Begin Source File

SOURCE=.\KeyInfoFrame.h
# End Source File
# Begin Source File

SOURCE=.\KeyInfoView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MarkDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarkSet.h
# End Source File
# Begin Source File

SOURCE=.\MathFormula.h
# End Source File
# Begin Source File

SOURCE=.\MruComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ParseOrgExFile.h
# End Source File
# Begin Source File

SOURCE=.\ProcessMgr.h
# End Source File
# Begin Source File

SOURCE=.\ReplaySetting.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetComDlg.h
# End Source File
# Begin Source File

SOURCE=.\SizingControlBar.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestPointsView.h
# End Source File
# Begin Source File

SOURCE=.\TPAddrDescDlg.h
# End Source File
# Begin Source File

SOURCE=.\TPAddrDescItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\XListCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cur_arro.cur
# End Source File
# Begin Source File

SOURCE=.\res\DspLogger.ico
# End Source File
# Begin Source File

SOURCE=.\res\DspLogger.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DspLoggerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\filter.bmp
# End Source File
# Begin Source File

SOURCE=.\res\find.bmp
# End Source File
# Begin Source File

SOURCE=.\res\find.ico
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

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_char.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_keyi.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_rcv_.ico
# End Source File
# Begin Source File

SOURCE=.\res\keyinfo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Rssi.ico
# End Source File
# Begin Source File

SOURCE=.\res\RssiBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RxBuffer.ico
# End Source File
# End Group
# Begin Group "Graph"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\graph\coords.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\coords.h
# End Source File
# Begin Source File

SOURCE=..\graph\f2doff.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\f2doff.h
# End Source File
# Begin Source File

SOURCE=..\graph\gen_prop_dlgs.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\gen_prop_dlgs.h
# End Source File
# Begin Source File

SOURCE=..\graph\graph_general.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\graph_general.h
# End Source File
# Begin Source File

SOURCE=..\graph\graph_props.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\graph_props.h
# End Source File
# Begin Source File

SOURCE=..\graph\graphcombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\graphcombobox.h
# End Source File
# Begin Source File

SOURCE=..\graph\GraphContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\GraphContainer.h
# End Source File
# Begin Source File

SOURCE=..\graph\GraphFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\GraphFrame.h
# End Source File
# Begin Source File

SOURCE=..\graph\graphpanel.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\graphpanel.h
# End Source File
# Begin Source File

SOURCE=..\graph\GraphWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\GraphWnd.h
# End Source File
# Begin Source File

SOURCE=..\graph\grres.h
# End Source File
# Begin Source File

SOURCE=..\graph\gruler.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\gruler.h
# End Source File
# Begin Source File

SOURCE=..\graph\points.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\points.h
# End Source File
# Begin Source File

SOURCE=..\graph\pviewbar.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\pviewbar.h
# End Source File
# Begin Source File

SOURCE=..\graph\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=..\graph\sizecbar.h
# End Source File
# End Group
# Begin Group "LUA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\LUA\lauxlib.h
# End Source File
# Begin Source File

SOURCE=..\LUA\lua.h
# End Source File
# Begin Source File

SOURCE=..\LUA\LUA_FUNC.cpp
# End Source File
# Begin Source File

SOURCE=..\LUA\LUA_FUNC.h
# End Source File
# Begin Source File

SOURCE=..\LUA\luaconf.h
# End Source File
# Begin Source File

SOURCE=..\LUA\lualib.h
# End Source File
# End Group
# Begin Group "XMLParse"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XMLite.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLite.h
# End Source File
# Begin Source File

SOURCE=.\XmlParse.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlParse.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
