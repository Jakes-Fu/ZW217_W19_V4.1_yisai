# Microsoft Developer Studio Project File - Name="MobileCnnt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MobileCnnt - Win32 EGO Debug_UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MobileCnnt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MobileCnnt.mak" CFG="MobileCnnt - Win32 EGO Debug_UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MobileCnnt - Win32 Debug_UNICODE" (based on "Win32 (x86) Application")
!MESSAGE "MobileCnnt - Win32 Release_UNICODE" (based on "Win32 (x86) Application")
!MESSAGE "MobileCnnt - Win32 EGO Debug_UNICODE" (based on "Win32 (x86) Application")
!MESSAGE "MobileCnnt - Win32 EGO Release_UNICODE" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MobileCnnt"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MobileCnnt - Win32 Debug_UNICODE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MobileCnnt___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "MobileCnnt___Win32_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MobileCnnt___Win32_Debug_UNICODE"
# PROP Intermediate_Dir "MobileCnnt___Win32_Debug_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "effect\\" /I "include\\" /I "bin\\" /I "lib\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "effect\\" /I "..\..\include\\" /I "..\..\lib\\" /I "..\scimmipb" /I "sprite\\" /I "..\..\..\..\common\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "SPRD8800" /D "_USE_WMI" /D "_USE_LOG" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lib\uskin.lib /nologo /subsystem:windows /debug /machine:I386 /out:"bin/MobileCnnt.exe" /pdbtype:sept
# ADD LINK32 ..\..\lib\uskin.lib Version.lib ..\..\lib\HtmlHelp.lib SCIPortUD.lib rasapi32.lib shlwapi.lib SCIPBUD.lib SCIPBGroupUD.lib SCIMemoUD.lib SCIShortMsgUD.lib CrashReportd.lib Setupapi.lib  pdh.lib  /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:"../../../../Common/bin/pdb/SPRD8800.pdb" /debug /machine:I386 /out:"..\..\..\..\common\bin\SPRD8800\SPRD8800.exe" /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MobileCnnt - Win32 Release_UNICODE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MobileCnnt___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "MobileCnnt___Win32_Release_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MobileCnnt___Win32_Release_UNICODE"
# PROP Intermediate_Dir "MobileCnnt___Win32_Release_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "effect\\" /I "..\..\include\\" /I "..\..\lib\\" /I "..\scimmipb" /I "sprite\\" /I "..\..\..\..\common\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "SPRD8800" /D "_USE_WMI" /D "_USE_LOG" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\lib\uskin.lib Version.lib ..\..\lib\HtmlHelp.lib SCIPortU.lib EtionU.lib rasapi32.lib shlwapi.lib SCIPBU.lib SCIPBGroupU.lib SCIMemoU.lib SCIShortMsgU.lib CrashReport.lib Setupapi.lib  pdh.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:"../../../../Release/SPRD8800Tool/Bin/SPRD8800.pdb" /debug /machine:I386 /out:"..\..\..\..\Release\SPRD8800Tool\Bin\SPRD8800.exe" /libpath:"../../../../Common/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MobileCnnt - Win32 EGO Debug_UNICODE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MobileCnnt___Win32_EGO_Debug_UNICODE"
# PROP BASE Intermediate_Dir "MobileCnnt___Win32_EGO_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MobileCnnt___Win32_EGO_Debug_UNICODE"
# PROP Intermediate_Dir "MobileCnnt___Win32_EGO_Debug_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "effect\\" /I "..\..\include\\" /I "..\..\lib\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "SPRD8800" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "effect\\" /I "..\..\include\\" /I "..\..\lib\\" /I "..\scimmipb" /I "sprite\\" /I "..\..\..\..\common\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_EGO" /D "_USE_WMI" /D "_USE_LOG" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\lib\uskin.lib Version.lib ..\..\lib\HtmlHelp.lib SCIPortUD.lib rasapi32.lib shlwapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\common\bin\SPRD8800\SPRD8800.exe" /pdbtype:sept /libpath:"../../../../Common/Lib"
# ADD LINK32 ..\..\lib\uskin.lib Version.lib ..\..\lib\HtmlHelp.lib SCIPortUD.lib rasapi32.lib shlwapi.lib SCIMMIPBUD.lib SCIShortMsgUD.lib SCIMemoUD.lib SCIPBGroupUD.lib SCIMMIPBUD.lib Setupapi.lib pdh.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\common\bin\eGo\eGo.exe" /pdbtype:sept /libpath:"../../../../Common/Lib"

!ELSEIF  "$(CFG)" == "MobileCnnt - Win32 EGO Release_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MobileCnnt___Win32_EGO_Release_UNICODE"
# PROP BASE Intermediate_Dir "MobileCnnt___Win32_EGO_Release_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MobileCnnt___Win32_EGO_Release_UNICODE"
# PROP Intermediate_Dir "MobileCnnt___Win32_EGO_Release_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "effect\\" /I "..\..\include\\" /I "..\..\lib\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "SPRD8800" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "effect\\" /I "..\..\include\\" /I "..\..\lib\\" /I "..\scimmipb" /I "sprite\\" /I "..\..\..\..\common\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_EGO" /D "_USE_WMI" /D "_USE_LOG" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\lib\uskin.lib Version.lib ..\..\lib\HtmlHelp.lib SCIPortU.lib rasapi32.lib shlwapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\..\..\..\common\bin\SPRD8800\SPRD8800.exe" /libpath:"../../../../Common/Lib"
# ADD LINK32 ..\..\lib\uskin.lib Version.lib ..\..\lib\HtmlHelp.lib SCIPortU.lib rasapi32.lib shlwapi.lib SCIMMIPBU.lib SCIShortMsgU.lib SCIMemoU.lib SCIPBGroupU.lib SCIMMIPBU.lib Setupapi.lib  pdh.lib  /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\..\..\..\Release\eGo\bin\eGo.exe" /libpath:"../../../../Common/Lib"

!ENDIF 

# Begin Target

# Name "MobileCnnt - Win32 Debug_UNICODE"
# Name "MobileCnnt - Win32 Release_UNICODE"
# Name "MobileCnnt - Win32 EGO Debug_UNICODE"
# Name "MobileCnnt - Win32 EGO Release_UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ATCmdPage.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\BaseTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CallView.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\CategoryView.cpp
# End Source File
# Begin Source File

SOURCE=.\CgdcontPage.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\ChildFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigView.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectView.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\ContentView.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolButton.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\CoolToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DialUpView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDialNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHsdpaVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgINputPin1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputPuk1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOthers.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPBType.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSecurity.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSelGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSelNum.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSelSim.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSendPrg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSMSStore.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSyncPB.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTDGSM.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUnlockPin2.cpp
# End Source File
# Begin Source File

SOURCE=.\DspPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EngineeringView.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumPorts.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Sprite\FileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\FileNameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\HeaderBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoListView.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoListView_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoListView_Memo.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoListView_PB.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoListView_SMS.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InforListView_PBG.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoWrapper_Memo.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoWrapper_pb.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoWrapper_pbg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoWrapper_SMS.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\MemoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MobileCnnt.cpp
# End Source File
# Begin Source File

SOURCE=.\MobileCnnt.rc
# End Source File
# Begin Source File

SOURCE=.\MobileCnntDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MobileCnntView.cpp
# End Source File
# Begin Source File

SOURCE=.\ModemInfoClass.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyPin.cpp
# End Source File
# Begin Source File

SOURCE=.\MSInfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\PBDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\PbgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\Phone.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefsStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\ProgThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RasSpdM.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SautoattPage.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\SizingControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSCenterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSDlg_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSDoc_Edit.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSDoc_Option.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSPresetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSSendModeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SMSView.cpp
# End Source File
# Begin Source File

SOURCE=.\SpfrqPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SpidPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SpLog.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\SppsratePage.cpp
# End Source File
# Begin Source File

SOURCE=.\SptestPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SpuplmnPage.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sprite\Storage.cpp
# End Source File
# Begin Source File

SOURCE=.\SysconfigPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\ToolTipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\VCalendar.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite\VCard.cpp
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\effect\XStatic.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ATCmdPage.h
# End Source File
# Begin Source File

SOURCE=.\effect\BaseTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CallView.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\CategoryView.h
# End Source File
# Begin Source File

SOURCE=.\CgdcontPage.h
# End Source File
# Begin Source File

SOURCE=.\effect\childframewnd.h
# End Source File
# Begin Source File

SOURCE=.\ConnectView.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\ContentView.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolBar.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolButton.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolDlg.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolEdit.h
# End Source File
# Begin Source File

SOURCE=.\CoolFormView.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolMenu.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolStatic.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\effect\CoolToolBar.h
# End Source File
# Begin Source File

SOURCE=.\DialUpView.h
# End Source File
# Begin Source File

SOURCE=.\DlgDialNetwork.h
# End Source File
# Begin Source File

SOURCE=.\DlgHsdpaVersion.h
# End Source File
# Begin Source File

SOURCE=.\DlgINputPin1.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputPuk1.h
# End Source File
# Begin Source File

SOURCE=.\DlgOthers.h
# End Source File
# Begin Source File

SOURCE=.\DlgPBType.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgProgress.h
# End Source File
# Begin Source File

SOURCE=.\DlgSecurity.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectNetwork.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSelGroup.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSelNum.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSelSim.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSendPrg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSMSStore.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\DlgSyncPB.h
# End Source File
# Begin Source File

SOURCE=.\DlgTDGSM.h
# End Source File
# Begin Source File

SOURCE=.\DlgUnlockPin2.h
# End Source File
# Begin Source File

SOURCE=.\DspPage.h
# End Source File
# Begin Source File

SOURCE=.\EngineeringView.h
# End Source File
# Begin Source File

SOURCE=.\EnumPorts.h
# End Source File
# Begin Source File

SOURCE=..\Etion\EtionImp.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\FileDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\FileNameDlg.h
# End Source File
# Begin Source File

SOURCE=.\effect\HeaderBar.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoListView.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\InfoWrapper.h
# End Source File
# Begin Source File

SOURCE=.\ISpLog.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\MemoDlg.h
# End Source File
# Begin Source File

SOURCE=.\MobileCnnt.h
# End Source File
# Begin Source File

SOURCE=.\MobileCnntDoc.h
# End Source File
# Begin Source File

SOURCE=.\MobileCnntView.h
# End Source File
# Begin Source File

SOURCE=.\ModemInfoClass.h
# End Source File
# Begin Source File

SOURCE=.\ModifyPin.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\MsgIDDef.h
# End Source File
# Begin Source File

SOURCE=.\MSInfoView.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\PBDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\PbgDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\Phone.h
# End Source File
# Begin Source File

SOURCE=.\PrefsStatic.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\ProgThread.h
# End Source File
# Begin Source File

SOURCE=.\RasSpdM.h
# End Source File
# Begin Source File

SOURCE=.\RegistryEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SautoattPage.h
# End Source File
# Begin Source File

SOURCE=.\effect\SizingControlBar.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSCenterDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSDlg.h
# End Source File
# Begin Source File

SOURCE=.\SMSDoc.h
# End Source File
# Begin Source File

SOURCE=.\SMSFrame.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSPresetDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\SMSSendModeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SMSView.h
# End Source File
# Begin Source File

SOURCE=.\SpfrqPage.h
# End Source File
# Begin Source File

SOURCE=.\SpidPage.h
# End Source File
# Begin Source File

SOURCE=.\SpLog.h
# End Source File
# Begin Source File

SOURCE=.\SppsratePage.h
# End Source File
# Begin Source File

SOURCE=.\SptestPage.h
# End Source File
# Begin Source File

SOURCE=.\SpuplmnPage.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\Storage.h
# End Source File
# Begin Source File

SOURCE=.\SysconfigPage.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\ToolTipDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\VCalendar.h
# End Source File
# Begin Source File

SOURCE=.\Sprite\VCard.h
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\effect\XStatic.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_category.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_click.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_hover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\coming_c.ico
# End Source File
# Begin Source File

SOURCE=.\res\cur_horz.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_vert.cur
# End Source File
# Begin Source File

SOURCE=.\res\dis_start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlg_back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlg_backgrnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlg_clsbtn_click.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlg_clsbtn_hover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlg_clsbtn_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlg_title.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drag_mult.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drag_one.bmp
# End Source File
# Begin Source File

SOURCE=.\res\high.ico
# End Source File
# Begin Source File

SOURCE=.\res\higher.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico0000.ico
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

SOURCE=.\res\ico0001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_sig0.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_sig1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_sig2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_sig3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_sig4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_sig5.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\MobileCnnt.ico
# End Source File
# Begin Source File

SOURCE=.\res\MobileCnnt.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MobileCnntDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\new_start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\signal0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\signal1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sms_ego.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trademark.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
