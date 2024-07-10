# Microsoft Developer Studio Project File - Name="AudioTester" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AudioTester - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AudioTester.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AudioTester.mak" CFG="AudioTester - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AudioTester - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AudioTester - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "AudioTester - Win32 Test Debug" (based on "Win32 (x86) Application")
!MESSAGE "AudioTester - Win32 Debug Unicode" (based on "Win32 (x86) Application")
!MESSAGE "AudioTester - Win32 Release Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "AudioTester"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AudioTester - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O1 /I "..\Include" /I "..\..\..\..\Common\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Channel.lib DiagChan.lib Ws2_32.lib version.lib setupapi.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /libpath:"..\..\..\..\Common\Lib"

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\Include" /I "..\..\..\..\Common\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Channeld.lib DiagChand.lib Ws2_32.lib version.lib setupapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /pdbtype:sept /libpath:"..\..\..\..\Common\Lib"

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Test Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AudioTester___Win32_Test_Debug"
# PROP BASE Intermediate_Dir "AudioTester___Win32_Test_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AudioTester___Win32_Test_Debug"
# PROP Intermediate_Dir "AudioTester___Win32_Test_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\Common\Include" /I "..\..\..\..\..\MS_Code\Common\Include" /I "..\..\..\..\..\MS_Code\Ref\Include" /I "..\..\..\..\..\MS_Code\Rtos\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "EXTEND_DATA" /D "ROCKY" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\Include" /I "..\..\..\..\Common\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "SIMU_TEST" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Diagd.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /pdbtype:sept /libpath:"..\..\..\..\Common\Lib"
# ADD LINK32 Channeld.lib DiagChand.lib Ws2_32.lib version.lib setupapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /pdbtype:sept /libpath:"..\..\..\..\Common\Lib"

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Debug Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AudioTester___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "AudioTester___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AudioTester___Win32_Debug_Unicode"
# PROP Intermediate_Dir "AudioTester___Win32_Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\..\..\Common\Include" /I "..\..\..\..\..\MS_Code\common\export\Inc" /I "..\..\..\..\..\MS_Code\ms_ref\export\inc" /I "..\..\..\..\..\MS_Code\Rtos\export\Inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "..\Include" /I "..\..\..\..\Common\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Diagd.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /pdbtype:sept /libpath:"..\..\..\..\Common\Lib"
# ADD LINK32 Channeld.lib DiagChand.lib Ws2_32.lib version.lib setupapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /pdbtype:sept /libpath:"..\..\..\..\Common\Lib"

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Release Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioTester___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "AudioTester___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AudioTester___Win32_Release_Unicode"
# PROP Intermediate_Dir "AudioTester___Win32_Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O1 /I "..\..\..\..\Common\Include" /I "..\..\..\..\..\MS_Code\common\export\Inc" /I "..\..\..\..\..\MS_Code\ms_ref\export\inc" /I "..\..\..\..\..\MS_Code\Rtos\export\Inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /Od /I "..\Include" /I "..\..\..\..\Common\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Diag.lib version.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /libpath:"..\..\..\..\Common\Lib"
# ADD LINK32 Channel.lib DiagChan.lib Ws2_32.lib version.lib setupapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\..\..\..\Common\Bin\AudioTester.exe" /libpath:"..\..\..\..\Common\Lib"
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\Diagnostic\AudioTester\Documents\Specifications\AudioTester User Guide.doc" "..\..\..\..\Release\AudioTester\Doc\AudioTester User Guide(cn).doc"	copy "..\..\..\..\Diagnostic\AudioTester\Documents\En\AudioTester User Guide.doc" "..\..\..\..\Release\AudioTester\Doc\AudioTester User Guide(en).doc"	copy "..\..\..\..\common\bin\Channel.dll" "..\..\..\..\Release\AudioTester\Bin\Channel.dll"	copy "..\..\..\..\common\bin\channel.ini" "..\..\..\..\Release\AudioTester\Bin\channel.ini"	copy "..\..\..\..\common\bin\AudioTester.exe" "..\..\..\..\Release\AudioTester\Bin\AudioTester.exe"	copy "..\..\..\..\common\bin\AudioTester.ini" "..\..\..\..\Release\AudioTester\Bin\AudioTester.ini"	copy "..\..\..\..\common\bin\DiagChan.dll" "..\..\..\..\Release\AudioTester\Bin\DiagChan.dll"	copy "..\..\..\..\common\bin\showgraph.bmp" "..\..\..\..\Release\AudioTester\Bin\showgraph.bmp"	copy "..\Script\CtrlList.dat" "..\..\..\..\Release\AudioTester\Bin\CtrlList.dat"	copy "..\..\..\..\common\bin\showgraph.bmp" "..\..\..\..\Release\AudioTester\Bin\record_dp.bmp"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "AudioTester - Win32 Release"
# Name "AudioTester - Win32 Debug"
# Name "AudioTester - Win32 Test Debug"
# Name "AudioTester - Win32 Debug Unicode"
# Name "AudioTester - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdjustDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AecDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AgcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ALCStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioFileOpenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioModePage.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioStructHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioTester.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioTester.rc
# End Source File
# Begin Source File

SOURCE=.\AudioTesterSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BMPCtrlWithSpecRect.cpp
# End Source File
# Begin Source File

SOURCE=.\BuddiedSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CoefficientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvertFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlStatus_gen.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomDrawSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DigitalGainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlcSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAnalogGain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgArmVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgByte.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCNG.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEqCoefs.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEQContent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEQMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEQMode2.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEQMode3.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEQSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEqualizerSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLcfSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLineinGain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNxpRx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNxpTx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRcordDpLcf.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecordDp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecordDpEq.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecordDpInputGain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecorDpAlc.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTxNVParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DPLcfStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumPorts.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\EqDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EqGraphStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\EqModeStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\EqSetStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\excel.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Extend2Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Extend3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\fir_eq.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\FirAutoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FirAutoDlgEx.cpp
# End Source File
# Begin Source File

SOURCE=.\FrequencyGraphyStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\LcfStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyGainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicModePage.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCombox.cpp
# End Source File
# Begin Source File

SOURCE=.\MySpin.cpp
# End Source File
# Begin Source File

SOURCE=.\NGDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NumericEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherPicFormatToBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\PopDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordEqStc.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SliderCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StNgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VolumeCtrlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VolumeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XagcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XTabCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdjustDlg.h
# End Source File
# Begin Source File

SOURCE=.\AecDialog.h
# End Source File
# Begin Source File

SOURCE=.\AgcDlg.h
# End Source File
# Begin Source File

SOURCE=.\ALCStatic.h
# End Source File
# Begin Source File

SOURCE=.\AudioFileOpenDlg.h
# End Source File
# Begin Source File

SOURCE=.\AudioModePage.h
# End Source File
# Begin Source File

SOURCE=.\AudioStructHelper.h
# End Source File
# Begin Source File

SOURCE=.\AudioTester.h
# End Source File
# Begin Source File

SOURCE=.\AudioTesterSheet.h
# End Source File
# Begin Source File

SOURCE=.\BitmapCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BMPCtrlWithSpecRect.h
# End Source File
# Begin Source File

SOURCE=.\BuddiedSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CoefficientDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorListBox.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\ConvertFunc.h
# End Source File
# Begin Source File

SOURCE=.\CtrlStatus.h
# End Source File
# Begin Source File

SOURCE=.\CustomDrawSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DigitalGainDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlcSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgAnalogGain.h
# End Source File
# Begin Source File

SOURCE=.\DlgArmVolume.h
# End Source File
# Begin Source File

SOURCE=.\DlgByte.h
# End Source File
# Begin Source File

SOURCE=.\DlgCNG.h
# End Source File
# Begin Source File

SOURCE=.\DlgEqCoefs.h
# End Source File
# Begin Source File

SOURCE=.\DlgEQContent.h
# End Source File
# Begin Source File

SOURCE=.\DlgEQMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgEQMode2.h
# End Source File
# Begin Source File

SOURCE=.\DlgEQMode3.h
# End Source File
# Begin Source File

SOURCE=.\DlgEQSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgEqualizerSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgLcfSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgLineinGain.h
# End Source File
# Begin Source File

SOURCE=.\DlgNxpRx.h
# End Source File
# Begin Source File

SOURCE=.\DlgNxpTx.h
# End Source File
# Begin Source File

SOURCE=.\DlgRcordDpLcf.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecord.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecordDp.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecordDpEq.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecordDpInputGain.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecorDpAlc.h
# End Source File
# Begin Source File

SOURCE=.\DlgTxNVParam.h
# End Source File
# Begin Source File

SOURCE=.\DPLcfStatic.h
# End Source File
# Begin Source File

SOURCE=.\EnumPorts.h
# End Source File
# Begin Source File

SOURCE=.\EqDlg.h
# End Source File
# Begin Source File

SOURCE=.\EqGraphStatic.h
# End Source File
# Begin Source File

SOURCE=.\EqModeStatic.h
# End Source File
# Begin Source File

SOURCE=.\EqSetStatic.h
# End Source File
# Begin Source File

SOURCE=.\excel.h
# End Source File
# Begin Source File

SOURCE=.\ExtDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\Extend2Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Extend3Dlg.h
# End Source File
# Begin Source File

SOURCE=.\fir_eq.h
# End Source File
# Begin Source File

SOURCE=.\FirAutoDlg.h
# End Source File
# Begin Source File

SOURCE=.\FirAutoDlgEx.h
# End Source File
# Begin Source File

SOURCE=.\FrequencyGraphyStatic.h
# End Source File
# Begin Source File

SOURCE=.\HexEdit.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\LcfStatic.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\ModifyGainDlg.h
# End Source File
# Begin Source File

SOURCE=.\MusicModePage.h
# End Source File
# Begin Source File

SOURCE=.\MyCombox.h
# End Source File
# Begin Source File

SOURCE=.\MySpin.h
# End Source File
# Begin Source File

SOURCE=.\NGDlg.h
# End Source File
# Begin Source File

SOURCE=.\NumericEdit.h
# End Source File
# Begin Source File

SOURCE=.\OtherPicFormatToBMP.h
# End Source File
# Begin Source File

SOURCE=.\PopDialog.h
# End Source File
# Begin Source File

SOURCE=.\RecordEqStc.h
# End Source File
# Begin Source File

SOURCE=.\RecordStatic.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "AudioTester - Win32 Release"

# Begin Custom Build
IntDir=.\Release
InputPath=.\Resource.h

"$(IntDir)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	CScript ..\Script\GenId.vbs

# End Custom Build

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Debug"

# Begin Custom Build
IntDir=.\Debug
InputPath=.\Resource.h

"$(IntDir)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	CScript ..\Script\GenId.vbs

# End Custom Build

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Test Debug"

# Begin Custom Build
IntDir=.\AudioTester___Win32_Test_Debug
InputPath=.\Resource.h

"$(IntDir)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	CScript ..\Script\GenId.vbs

# End Custom Build

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Debug Unicode"

# Begin Custom Build
IntDir=.\AudioTester___Win32_Debug_Unicode
InputPath=.\Resource.h

"$(IntDir)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	CScript ..\Script\GenId.vbs

# End Custom Build

!ELSEIF  "$(CFG)" == "AudioTester - Win32 Release Unicode"

# Begin Custom Build
IntDir=.\AudioTester___Win32_Release_Unicode
InputPath=.\Resource.h

"$(IntDir)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	CScript ..\Script\GenId.vbs

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SettingsPage.h
# End Source File
# Begin Source File

SOURCE=.\SliderCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StNgDlg.h
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# Begin Source File

SOURCE=.\VolumeCtrlDlg.h
# End Source File
# Begin Source File

SOURCE=.\VolumeDlg.h
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\XagcDlg.h
# End Source File
# Begin Source File

SOURCE=.\XTabCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AudioTester.ico
# End Source File
# Begin Source File

SOURCE=.\res\AudioTester.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_arr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_c.bmp
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

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dspchip1.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dspchip2.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dspchip3.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dspchip4.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dspchip5.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dspchip6.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dsphw1.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dsphw2.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dspsw1.jpg
# End Source File
# Begin Source File

SOURCE=.\res\dspsw2.jpg
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_arr.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
