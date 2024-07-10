# Microsoft Developer Studio Project File - Name="BMPlatform" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BMPlatform - Win32 Cust Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BMPlatform.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BMPlatform.mak" CFG="BMPlatform - Win32 Cust Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BMPlatform - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BMPlatform - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BMPlatform - Win32 Cust Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BMPlatform - Win32 Cust Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BMPlatform"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BMPlatform - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\..\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "BMP_DLL" /D "NOT_USE_DIAGCHAN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"..\..\..\..\common\bin\BMPlatform.dll" /implib:"..\..\..\..\common\lib\BMPlatform.lib" /libpath:"..\..\..\..\common\lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "BMPlatform - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "BMP_DLL" /D "NOT_USE_DIAGCHAN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrtd.lib" /out:"..\..\..\..\common\bin\BMPlatform.dll" /implib:"..\..\..\..\common\lib\BMPlatform.lib" /pdbtype:sept /libpath:"..\..\..\..\common\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "BMPlatform - Win32 Cust Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BMPlatform___Win32_Cust_Release"
# PROP BASE Intermediate_Dir "BMPlatform___Win32_Cust_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BMPlatform___Win32_Cust_Release"
# PROP Intermediate_Dir "BMPlatform___Win32_Cust_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\..\Include" /I "../../../../../MS_Code/common/include" /I "../../../../../MS_Code/RTOS/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNCODE" /D "UNICODE" /D "_USRDLLBMP_DLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\..\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "BMP_DLL" /D "NOT_USE_DIAGCHAN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 BMPacket.lib Channel.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrtd.lib" /out:"..\..\..\..\common\bin\BMPlatform.dll" /libpath:"..\..\..\..\common\lib"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\BMPlatform.def" /out:"..\..\..\bin\BMPlatform.dll" /implib:"..\..\lib\BMPlatform.lib" /libpath:"..\..\lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "BMPlatform - Win32 Cust Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BMPlatform___Win32_Cust_Debug"
# PROP BASE Intermediate_Dir "BMPlatform___Win32_Cust_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BMPlatform___Win32_Cust_Debug"
# PROP Intermediate_Dir "BMPlatform___Win32_Cust_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "../../../../../MS_Code/common/include" /I "../../../../../MS_Code/RTOS/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "BMP_DLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /D "_USRDLL" /D "BMP_DLL" /D "NOT_USE_DIAGCHAN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 BMPacket.lib ChannelD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrtd.lib" /out:"..\..\..\..\common\bin\BMPlatform.dll" /pdbtype:sept /libpath:"..\..\..\..\common\lib"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\BMPlatform.def" /out:"..\..\..\bin\BMPlatform.dll" /implib:"..\..\lib\BMPlatform.lib" /pdbtype:sept /libpath:"..\..\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "BMPlatform - Win32 Release"
# Name "BMPlatform - Win32 Debug"
# Name "BMPlatform - Win32 Cust Release"
# Name "BMPlatform - Win32 Cust Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BMFileImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BMPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\BMPlatform.cpp
# End Source File
# Begin Source File

SOURCE=.\BMPlatform.def

!IF  "$(CFG)" == "BMPlatform - Win32 Release"

!ELSEIF  "$(CFG)" == "BMPlatform - Win32 Debug"

!ELSEIF  "$(CFG)" == "BMPlatform - Win32 Cust Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BMPlatform - Win32 Cust Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BMPlatform.rc
# End Source File
# Begin Source File

SOURCE=.\BootModeIntegOpr.cpp
# End Source File
# Begin Source File

SOURCE=.\BootModeObj.cpp
# End Source File
# Begin Source File

SOURCE=.\BootModeObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BootModeOpr.cpp
# End Source File
# Begin Source File

SOURCE=.\crc16.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\DLFile.cpp
# End Source File
# Begin Source File

SOURCE=.\IBMFile.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionHelpper.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketThread.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BMFileImpl.h
# End Source File
# Begin Source File

SOURCE=.\BMPackage.h
# End Source File
# Begin Source File

SOURCE=.\BMPlatform.h
# End Source File
# Begin Source File

SOURCE=.\BootModeIntegOpr.h
# End Source File
# Begin Source File

SOURCE=.\BootModeObj.h
# End Source File
# Begin Source File

SOURCE=.\BootModeObject.h
# End Source File
# Begin Source File

SOURCE=.\BootModeOpr.h
# End Source File
# Begin Source File

SOURCE=.\crc16.h
# End Source File
# Begin Source File

SOURCE=.\DLFile.h
# End Source File
# Begin Source File

SOURCE=.\OptionHelpper.h
# End Source File
# Begin Source File

SOURCE=.\PacketThread.h
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

SOURCE=.\res\BMPlatform.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
