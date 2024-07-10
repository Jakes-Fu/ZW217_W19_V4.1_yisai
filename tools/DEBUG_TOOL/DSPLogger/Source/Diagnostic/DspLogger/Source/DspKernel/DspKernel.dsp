# Microsoft Developer Studio Project File - Name="DspKernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DspKernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DspKernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DspKernel.mak" CFG="DspKernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DspKernel - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DspKernel - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DspKernel - Win32 Release_Audio" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "DspKernel"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DspKernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Common\Lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSPKERNEL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../SciPort" /I "..\..\..\..\common\lib" /I "..\..\..\..\Common\Bin" /I "..\..\..\..\Common\include" /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSPKERNEL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib channel.lib DiagChan.lib Ws2_32.lib /nologo /dll /pdb:"..\..\..\..\Release\DSPLogger\Bin\DspKernel.pdb" /debug /machine:I386 /out:"..\..\..\..\Common\Bin\DspKernel.dll" /libpath:"../sciport" /libpath:"..\..\..\..\common\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DspKernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSPKERNEL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "../SciPort" /I "..\..\..\..\common\lib" /I "..\..\..\..\Common\Bin" /I "..\..\..\..\Common\include" /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSPKERNEL_EXPORTS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib channelD.lib DiagChand.lib Ws2_32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Common\Bin\DspKerneld.dll" /pdbtype:sept /libpath:"../sciport" /libpath:"..\..\..\..\common\lib"

!ELSEIF  "$(CFG)" == "DspKernel - Win32 Release_Audio"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DspKernel___Win32_Release_Audio"
# PROP BASE Intermediate_Dir "DspKernel___Win32_Release_Audio"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DspKernel___Win32_Release_Audio"
# PROP Intermediate_Dir "DspKernel___Win32_Release_Audio"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../SciPort" /D "DSPLOGGER_2p5G" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSPKERNEL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp4 /MT /W4 /GX /O2 /I "../SciPort" /D "DSPLOGGER_AUDIO" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSPKERNEL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 SciPort.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /machine:I386 /out:"..\..\..\..\Common\Bin\Dsplogger_2p5G/DspKernel.dll" /libpath:"../sciport"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 SciPort.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib SciUartComm.lib /nologo /dll /machine:I386 /out:"..\..\..\..\Common\Bin\Dsplogger_Audio/DspKernel.dll" /libpath:"../sciport"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "DspKernel - Win32 Release"
# Name "DspKernel - Win32 Debug"
# Name "DspKernel - Win32 Release_Audio"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DspKernel.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessByte.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TimerHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TPSet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DspKernel.h
# End Source File
# Begin Source File

SOURCE=.\DspKernelExport.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\ProcessByte.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TimerHelper.h
# End Source File
# Begin Source File

SOURCE=.\TPSet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
