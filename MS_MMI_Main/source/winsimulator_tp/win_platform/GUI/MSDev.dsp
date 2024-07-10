# Microsoft Developer Studio Project File - Name="MSDev" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MSDev - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MSDev.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MSDev.mak" CFG="MSDev - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MSDev - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MSDev - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MSDev"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MSDev - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\bin\MSDev.exe"

!ELSEIF  "$(CFG)" == "MSDev - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../../../MS_Code/MS_Customize/export/inc" /I "../../../../../../MS_Code/mm_ref/source/drv_sc6800/h" /I "../../../../../../MS_Code/base/sim/source/h" /I "../../../../../../../MS_Code/base/L4/export/inc" /I "../../../../../../MS_Code/base/sim/export/inc" /I "../../../../../../MS_Code/base/layer1/export/inc" /I "../../../../../../MS_Code/base/ps/export/inc" /I "../../../../../../MS_Code/transform/export/inc" /I "../include" /I "../controlpanel_bl7" /I "../../../../../../tools/common/include" /I "../../../../../../ms_code/Ms_ref/export/inc" /I "../../../../../../ms_code/rtos/export/inc" /I "../../../../../../ms_code/common/export/inc" /I "../../../../../../ms_code/base/ps/source/sim/test/simconpad" /I "../../../../../../MS_Code/chip_drv/export/inc" /I "../../../../../../ms_code/chip_drv/export/inc/outdated" /I "./gdiplus/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_RTOS" /D "_FEATURE_BSSIM_TEST" /D "EXTERNAL_SDL_DEBUG" /D "LOG_DEBUG" /D "_TEMP_DEBUG" /D "SDL" /D "_MN_EVENT" /D "FLASH_EMU" /D "_MSSIM" /D "FEATURE_TEST" /D "_ENABLE_WATCHDOG" /D "_ENABLE_LOG_TOOL" /D "_SUPPORT_GPRS_" /D "MEMORY_DEBUG_CODE" /D "LOG_FLOW_CONTROL" /D "CONPAN_INTERNAL_USE" /D "WTELEPHONE" /D "_CONTROL_PANEL" /D "_PS_BL7" /D "MSCODE_MOD" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CommSockd.lib version.lib gdiplus.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\bin\MSDev.exe" /pdbtype:sept /libpath:"../bin" /libpath:"./gdiplus/lib"

!ENDIF 

# Begin Target

# Name "MSDev - Win32 Release"
# Name "MSDev - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ATCommandDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorUtility.cpp

!IF  "$(CFG)" == "MSDev - Win32 Release"

!ELSEIF  "$(CFG)" == "MSDev - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ComSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLayers.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMainLcdLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MSDev.cpp
# End Source File
# Begin Source File

SOURCE=.\MSDev.rc
# End Source File
# Begin Source File

SOURCE=.\MSDevDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MSSimIniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MSSimInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\PixelPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialCom.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
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

SOURCE=.\ColorUtility.h
# End Source File
# Begin Source File

SOURCE=.\ComSettings.h
# End Source File
# Begin Source File

SOURCE=.\CoolStatic.h
# End Source File
# Begin Source File

SOURCE=.\DlgLayers.h
# End Source File
# Begin Source File

SOURCE=.\DlgMainLcdLayer.h
# End Source File
# Begin Source File

SOURCE=.\HexEdit.h
# End Source File
# Begin Source File

SOURCE=.\ImageConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImgDialog.h
# End Source File
# Begin Source File

SOURCE=.\MonitorDlg.h
# End Source File
# Begin Source File

SOURCE=.\MSDev.h
# End Source File
# Begin Source File

SOURCE=.\MSDevDlg.h
# End Source File
# Begin Source File

SOURCE=.\MSSimIniFile.h
# End Source File
# Begin Source File

SOURCE=.\MSSimInterface.h
# End Source File
# Begin Source File

SOURCE=.\PixelPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialCom.h
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

SOURCE=.\res\hand1.cur
# End Source File
# Begin Source File

SOURCE=.\res\hand2.cur
# End Source File
# Begin Source File

SOURCE=.\res\MSDev.ico
# End Source File
# Begin Source File

SOURCE=.\res\MSDev.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\blackmask.png
# End Source File
# Begin Source File

SOURCE=.\res\overbutton.png
# End Source File
# Begin Source File

SOURCE=.\res\pressbutton.png
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
