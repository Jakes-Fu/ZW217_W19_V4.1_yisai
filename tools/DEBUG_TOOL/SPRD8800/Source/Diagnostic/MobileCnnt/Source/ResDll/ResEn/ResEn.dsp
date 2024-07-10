# Microsoft Developer Studio Project File - Name="ResEn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ResEn - Win32 EGO Debug_UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ResEn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ResEn.mak" CFG="ResEn - Win32 EGO Debug_UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ResEn - Win32 Debug_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ResEn - Win32 Release_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ResEn - Win32 EGO Release_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ResEn - Win32 EGO Debug_UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ResEn"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ResEn - Win32 Debug_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ResEn___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "ResEn___Win32_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../Common/Lib"
# PROP Intermediate_Dir "ResEn___Win32_Debug_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../../Common/Bin/MobileSprite/7550/ResEn.dll" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:"../../../../../Common/bin/pdb/ResEnUD.pdb" /debug /machine:I386 /def:".\ResEnUD.def" /out:"..\..\..\..\..\common\bin\SPRD8800\ResEnUD.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ResEn - Win32 Release_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ResEn___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "ResEn___Win32_Release_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../../Common/Lib"
# PROP Intermediate_Dir "ResEn___Win32_Release_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../../../../Common/Bin/MobileSprite/7550/ResEn.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:"../../../../../Release/SPRD8800Tool/Bin/ResEnU.pdb" /debug /machine:I386 /def:".\ResEnU.def" /out:"..\..\..\..\..\Release\SPRD8800Tool\Bin\ResEnU.dll" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ResEn - Win32 EGO Release_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ResEn___Win32_EGO_Release_UNICODE"
# PROP BASE Intermediate_Dir "ResEn___Win32_EGO_Release_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../../Common/Lib"
# PROP Intermediate_Dir "ResEn___Win32_EGO_Release_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\common\bin\SPRD8800\ResEn.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\ResEnU.def" /out:"..\..\..\..\..\Release\eGo\bin\ResEnU.dll" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ResEn - Win32 EGO Debug_UNICODE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ResEn___Win32_EGO_Debug_UNICODE"
# PROP BASE Intermediate_Dir "ResEn___Win32_EGO_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../Common/Lib"
# PROP Intermediate_Dir "ResEn___Win32_EGO_Debug_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\common\bin\SPRD8800\ResEn.dll" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\ResEnUD.def" /out:"..\..\..\..\..\common\bin\eGo\ResEnUD.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ResEn - Win32 Debug_UNICODE"
# Name "ResEn - Win32 Release_UNICODE"
# Name "ResEn - Win32 EGO Release_UNICODE"
# Name "ResEn - Win32 EGO Debug_UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ResEn.cpp
# End Source File
# Begin Source File

SOURCE=.\ResEn.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ResEn.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ResEn.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
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

SOURCE=.\res\bmp_category.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_efs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boc_click.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boc_hover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boc_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\browse_click.bmp
# End Source File
# Begin Source File

SOURCE=.\res\browse_hover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\browse_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_click.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_down_click.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_down_hover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_down_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_hover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_up_click.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_up_hover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btn_up_normal.bmp
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

SOURCE=.\res\MobileCnntDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\MobileSprite.ico
# End Source File
# Begin Source File

SOURCE=.\res\MobileSpriteDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\music.bmp
# End Source File
# Begin Source File

SOURCE=.\res\new_start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\picture.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ResEn.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ring.bmp
# End Source File
# Begin Source File

SOURCE=.\res\separator.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sms_ego.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar_24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar_24_disabled.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
