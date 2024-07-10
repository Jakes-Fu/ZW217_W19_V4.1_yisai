# Microsoft Developer Studio Project File - Name="NVEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NVEditor - Win32 Cust Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NVEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NVEditor.mak" CFG="NVEditor - Win32 Cust Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NVEditor - Win32 Cust Release" (based on "Win32 (x86) Application")
!MESSAGE "NVEditor - Win32 Cust Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "NVEditor"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NVEditor - Win32 Cust Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "NVEditor___Win32_Cust_Release"
# PROP BASE Intermediate_Dir "NVEditor___Win32_Cust_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NVEditor___Win32_Cust_Release"
# PROP Intermediate_Dir "NVEditor___Win32_Cust_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "..\..\..\PublicLibrary\BootMode\Include" /I "..\..\..\..\MS_Code\ms_ref\export\inc" /I "..\..\..\..\MS_Code\rtos\export\inc" /I "..\..\..\..\MS_Code\common\export\inc" /I "..\..\..\common\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ChanMan.lib Version.lib BMPlatform.lib shlwapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../common/bin/NVEditor.exe" /libpath:"../../../Common/lib"
# ADD LINK32 Version.lib BMPlatform.lib shlwapi.lib DiagChan.lib Ws2_32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../bin/NVEditor.exe" /libpath:"../lib"

!ELSEIF  "$(CFG)" == "NVEditor - Win32 Cust Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "NVEditor___Win32_Cust_Debug"
# PROP BASE Intermediate_Dir "NVEditor___Win32_Cust_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NVEditor___Win32_Cust_Debug"
# PROP Intermediate_Dir "NVEditor___Win32_Cust_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\..\..\MS_Code\PS\Include" /I "..\..\..\PublicLibrary\BootMode\Include" /I "..\..\..\..\MS_Code\ms_ref\export\inc" /I "..\..\..\..\MS_Code\rtos\export\inc" /I "..\..\..\..\MS_Code\common\export\inc" /I "..\..\..\common\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\include" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ChanMand.lib Version.lib BMPlatform.lib shlwapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../../common/bin/NVEditor.exe" /pdbtype:sept /libpath:"../../../common/lib/"
# ADD LINK32 Version.lib BMPlatform.lib shlwapi.lib DiagChan.lib Ws2_32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../bin/NVEditor.exe" /pdbtype:sept /libpath:"../lib"

!ENDIF 

# Begin Target

# Name "NVEditor - Win32 Cust Release"
# Name "NVEditor - Win32 Cust Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ComboInListView.cpp
# End Source File
# Begin Source File

SOURCE=.\CustView.cpp
# End Source File
# Begin Source File

SOURCE=.\DevHound.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagObj.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBMDloader.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExport.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\DLSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\EditItemID.cpp
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FileMap.cpp
# End Source File
# Begin Source File

SOURCE=.\FileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IHListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\LFPFeedBack.cpp
# End Source File
# Begin Source File

SOURCE=.\listeditctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\md5c.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MyHSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MySuperGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\NewItem.cpp
# End Source File
# Begin Source File

SOURCE=.\NewItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\NewModule.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProject.cpp
# End Source File
# Begin Source File

SOURCE=.\NumericEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\NVEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\NVEditor.rc
# End Source File
# Begin Source File

SOURCE=.\NVEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\NVEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketModerm.cpp
# End Source File
# Begin Source File

SOURCE=.\PasteSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PortSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PromptInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadFlashDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.cpp
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

SOURCE=.\StiSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\supergridctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ComboInListView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\Include\Common.h
# End Source File
# Begin Source File

SOURCE=.\CustView.h
# End Source File
# Begin Source File

SOURCE=.\DevHound.h
# End Source File
# Begin Source File

SOURCE=.\DiagObj.h
# End Source File
# Begin Source File

SOURCE=.\DlgBMDloader.h
# End Source File
# Begin Source File

SOURCE=.\DlgExport.h
# End Source File
# Begin Source File

SOURCE=.\DlgFindItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgFunctions.h
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.h
# End Source File
# Begin Source File

SOURCE=.\DLSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\EditItemID.h
# End Source File
# Begin Source File

SOURCE=.\EdtBtnCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FileMap.h
# End Source File
# Begin Source File

SOURCE=.\FileParser.h
# End Source File
# Begin Source File

SOURCE=.\FlashDlg.h
# End Source File
# Begin Source File

SOURCE=.\IHListBox.h
# End Source File
# Begin Source File

SOURCE=.\LFPFeedBack.h
# End Source File
# Begin Source File

SOURCE=.\listeditctrl.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyHSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\MySuperGrid.h
# End Source File
# Begin Source File

SOURCE=.\NewItem.h
# End Source File
# Begin Source File

SOURCE=.\NewItemInfo.h
# End Source File
# Begin Source File

SOURCE=.\NewModule.h
# End Source File
# Begin Source File

SOURCE=.\NewProject.h
# End Source File
# Begin Source File

SOURCE=.\NumericEdit.h
# End Source File
# Begin Source File

SOURCE=.\NVEditor.h
# End Source File
# Begin Source File

SOURCE=.\NVEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\NVEditorView.h
# End Source File
# Begin Source File

SOURCE=..\Include\NVItemDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\Include\NVValueFile.h
# End Source File
# Begin Source File

SOURCE=.\PasteSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\PortSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\PromptInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReadFlashDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.h
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

SOURCE=.\StiSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\supergridctrl.h
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\error.ico
# End Source File
# Begin Source File

SOURCE=.\res\folders.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NVEditor.ico
# End Source File
# Begin Source File

SOURCE=.\res\NVEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\NVEditorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\subitems.bmp
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
