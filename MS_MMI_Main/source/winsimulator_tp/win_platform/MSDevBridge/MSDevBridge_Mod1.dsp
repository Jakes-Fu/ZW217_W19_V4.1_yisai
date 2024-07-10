# Microsoft Developer Studio Project File - Name="MSDevBridge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MSDevBridge - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MSDevBridge_Mod.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MSDevBridge_Mod.mak" CFG="MSDevBridge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MSDevBridge - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MSDevBridge - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MSDevBridge"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MSDevBridge - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../bin/MSDevBridge.lib"

!ELSEIF  "$(CFG)" == "MSDevBridge - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "../../../../../common/export/inc" /I "../include" /I "../../../../../ms_ref/export/inc" /I "../../../../../transform/export/inc" /I "../mskernelapi" /I "../../../../../rtos/export/inc" /I "../../../../../ps/include" /I "../../../../../chip_drv/export/inc" /I "../../../../../chip_drv/export/inc/outdated" /I "../../../../../ms_ref/source/display/inc" /I "../../../../../ms_ref/source/lcd/inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_RTOS" /D "_FEATURE_BSSIM_TEST" /D "EXTERNAL_SDL_DEBUG" /D "LOG_DEBUG" /D "SDL" /D "FLASH_EMU" /D "_MSSIM" /D "FEATURE_TEST" /D "SIM_TEST_IN_WIN32" /D "_ENABLE_LOG_TOOL" /D "_CONTROL_PANEL" /D "_PS_BL7" /D "_REF_SC6800_" /D "MSCODE_MOD" /D "DRECORDER_REF_MOD" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../bin/MSDevBridge.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=ATTRIB -R ../Bin/MSDevBridge.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "MSDevBridge - Win32 Release"
# Name "MSDevBridge - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BridgeLayer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
